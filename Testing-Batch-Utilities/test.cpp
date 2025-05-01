/*
 * EzTCObject.cpp
 *
 *  Created on: 20.12.2016
 *      Author: fox0n3j
 */

#include "EzTCObject.hxx"
#include "EzTCUser.hxx"
#include "EzTCGroup.hxx"
#include "ezx_itk_wrapper.hxx"
#include "EzTCMarkpoint.hxx"
#include "EzTCLogger.hxx"
#include "EzInvalidArgumentException.hxx"
#include "EzPropertyBuilder.hxx"
#include "EzTCDate.hxx"
#include "EzTCRelation.hxx"
#include "EzTCTypeAnalyser.hxx"

#include "boost/format.hpp"
#include "boost/foreach.hpp"

#include "EzXErrorConstants.hxx"

#include <vector>
#include <sstream>
#include <string>

using namespace ezx::itk;
using namespace ezx::error;
using namespace ezx::datamodelanalyser;
using namespace std;
using namespace ezx::logger;
using namespace ezx::tc::system;

namespace ezx {
	namespace tc {
		namespace datamodel {

			const std::string EzTCObject::loggerModule = "ezx.tc.datamodel.eztcobject";
			const std::string EzTCObject::TCType = EZX_TYPE_POM_OBJECT;

			EzTCObject::EzTCObject() :
				EzTCBusinessObject()
			{}

			EzTCObject::EzTCObject(tag_t object)
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				checkObjectNotNullAndTypeIsValid(object, TCType);
			}

			EzTCObject::~EzTCObject()
			{
#if 0
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);

				if (!isNulltag() && isValid()) {
					logger.trace(__FILE_LINE__, boost::format("~type=[%s]") % getTCTypeName());
					if (isPersistent() && isLoaded()) {
						try {
							if (isLocked()) {
								logger.trace(__FILE_LINE__, boost::format("destructor called but object %s was still locked") % toString());
							}
						}
						catch (std::exception& ex) {
							TCLOG_ERROR(logger, str(boost::format("EzTCObject::~EzTCObject: exception thrown:%s") % ex.what()));
						}
					}
				}
#endif
			}

			bool EzTCObject::isLocked() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);

				if (m_tag != NULLTAG) {
					int lock_token = 0;
					EZXITK_POM_ask_instance_lock(m_tag, lock_token);
					return lock_token == POM_modify_lock;
				}
				return false;
			}

			void EzTCObject::lock() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");
				if (!isLocked()) {
					EZXITK_AOM_refresh(m_tag, POM_modify_lock);
				}
				else {
					logger.trace(__FILE_LINE__, boost::format("lock called but object %s was already locked") % toString());
				}
			}

			void EzTCObject::unlock() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");
				if (isLocked()) {
					EZXITK_AOM_refresh(m_tag, POM_no_lock);
				}
				else {
					logger.trace(__FILE_LINE__, boost::format("unlock called but object %s was not locked") % toString());
				}
			}

			void EzTCObject::save() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");
				EZXITK_AOM_save_with_extensions(m_tag);
			}

			vector<EzTCObject> EzTCObject::getRelatedSecondaryObjects(const string& relationType, const string& secondaryType) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);

				tag_t relationTypeTag = NULLTAG;
				if (!relationType.empty()) {
					try {
						EZXITK_GRM_find_relation_type(relationType, relationTypeTag);
					}
					catch (EzTCException& ex) {
						throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: relationType " + relationType + " not found");
					}
				}

				if (!secondaryType.empty()) {
					if (!EzTCTypeAnalyser::isTypeExists(secondaryType)) {
						throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: secondaryType " + secondaryType + " not found");
					}
				}

				vector<tag_t> secondaryObjectTags;
				EZXITK_GRM_list_secondary_objects_only(m_tag, relationTypeTag, secondaryObjectTags);

				vector<EzTCObject> secondaryObjects;
				BOOST_FOREACH(tag_t secTag, secondaryObjectTags) {
					EzTCObject secondaryObject(secTag);
					if (secondaryType.empty() || secondaryType == secondaryObject.getTCTypeName()) {
						secondaryObjects.push_back(secondaryObject);
					}
				}
				return secondaryObjects;
			}

			vector<EzTCObject> EzTCObject::getRelatedPrimaryObjects(const string& relationType, const string& primaryType) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);

				tag_t relationTypeTag = NULLTAG;
				if (!relationType.empty()) {
					try {
						EZXITK_GRM_find_relation_type(relationType, relationTypeTag);
					}
					catch (EzTCException& ex) {
						throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: relationType " + relationType + " not found");
					}
				}

				if (!primaryType.empty()) {
					if (!EzTCTypeAnalyser::isTypeExists(primaryType)) {
						throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: primaryType " + primaryType + " not found");
					}
				}

				vector<tag_t> primaryObjectTags;
				EZXITK_GRM_list_primary_objects_only(m_tag, relationTypeTag, primaryObjectTags);
				logger.info(__FILE_LINE__, boost::format("%s number of primary objects found with %s relation") % primaryObjectTags.size() % relationType);

				vector<EzTCObject> primaryObjects;
				BOOST_FOREACH(tag_t primTag, primaryObjectTags) {
					EzTCObject primaryObject(primTag);
					if (primaryType.empty() || primaryType == primaryObject.getTCTypeName()) {
						primaryObjects.push_back(primaryObject);
					}
				}
				return primaryObjects;
			}


			void EzTCObject::deleteFromDatabase() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");

				try {
					//Trying to delete item here with the help of AOM_delete. in case of any failure catch block will be executed.
					EZXITK_AOM_lock_for_delete(m_tag);
					logger.info(__FILE_LINE__, boost::format("%s object locked for deletion") % m_tag);
					EZXITK_AOM_delete(m_tag);
					//When the object is deleted successfully it no more exists
					m_tag = NULLTAG;
				}
				catch (EzTCException& ex)
				{
					//Matching base error code with exeception error code.
					if (ex.askCode() == POM_inst_referenced)
					{
						string referenced;
						//Findind all the referencers of target business object.
						vector<EzTCObject> referenced_objects = getWhereReferenced(POM_in_ds_and_db);
						for (vector<EzTCObject>::iterator itr = referenced_objects.begin(); itr != referenced_objects.end(); itr++)
						{
							//Getting the class of the referencer in the string.
							string ClassOfReferencer = itr->getClassName();
							if (ClassOfReferencer == "ItemRevision" || ClassOfReferencer == "ImanRelation" || ClassOfReferencer == "Fnd0PartDesignAttrs" || ClassOfReferencer == "Part_0_Revision_alt")
							{
								//Deleting the referencers only if the above class match.
								itr->deleteFromDatabase();
							}
							else if (itr->getClassName() == "Folder")
							{
								//If the class of the referencer is Folder then removing the target object from content property of the folder.Just to make the target object ready for deletion.

								string UidOfTargetObj, UidOfObjectinFolder;
								vector<tag_t> prop;

								EZXITK_AOM_ask_value_tags(itr->m_tag, "contents", prop);

								//Finding UID of Target Business Object that is to be deleted. 
								EZXITK_POM_tag_to_uid(m_tag, UidOfTargetObj);
								for (long long int i1 = 0; i1 < prop.size(); i1++)
								{
									//Finding UID of Object inside the folder
									EZXITK_POM_tag_to_uid(prop[i1], UidOfObjectinFolder);
									if (UidOfTargetObj == UidOfObjectinFolder)
									{
										EZXITK_FL_remove(itr->m_tag, m_tag);
										EZXITK_AOM_save_with_extensions(itr->m_tag);
									}
								}
							}
							else
							{
								// No change is required in any other Condition here .
							}
						}
						try
						{
							//Again trying to delete the target object.
							EZXITK_AOM_lock_for_delete(m_tag);
							logger.info(__FILE_LINE__, boost::format("%s object locked for deletion") % m_tag);
							EZXITK_AOM_delete(m_tag);
							//When the object is deleted successfully and no more exists.
							m_tag = NULLTAG;
						}
						catch (const std::exception&)
						{
							//	In worst case if deletion fails then handling with the error messages. 

							//Unlocking the target object in case of deletion failure
							EZXITK_AOM_refresh(m_tag, 0);
							BOOST_FOREACH(EzTCObject referenced_object, referenced_objects)
							{
								try {
									referenced += referenced_object.toString() + ";";
									logger.info(__FILE_LINE__, boost::format("Instance of object found for deletion is refrenced to %s") % referenced_object.toUID());
								}
								catch (std::exception& ex2) {
									// make sure exceptions are caught for message (don't throw unwanted exception in catch)
									logger.debug(__FILE_LINE__, boost::format("ignoring toString() exception for uid %1%") % referenced_object.toUID());
								}
							}
							EzMessageManager::getInstance().push(EzMessageManager::MESSAGE_SEVERITY_ERROR, EZX_CAN_NOT_BE_DELETED, toString(), referenced);
							throw EzTCException(__FILE_LINE__, EZX_CAN_NOT_BE_DELETED);
						}
					}
					else
					{
						// simply rethrow errors other than "instance is referenced"
						const char* exception_msg = ex.askCode() ? ex.askExceptionMessage().c_str() : "Unknown Exception occurred.";
						logger.error(__FILE_LINE__, boost::format("%s") % exception_msg);
						throw;
					}
				}
				catch (exception& ex)
				{
					// Generic exception handling.
					logger.error(__FILE_LINE__, boost::format("%s") % ex.what());
					throw;
				}
			}

			void EzTCObject::refresh() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");

				EZXITK_AOM_refresh(m_tag, POM_no_lock);
			}

			std::string EzTCObject::toUID() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);

				std::string uid;
				EZXITK_POM_tag_to_uid(m_tag, uid);
				return uid;
			}

			EzTCObject EzTCObject::fromUID(const std::string& uid)
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkNotEmpty(__FILE_LINE__, uid, "uid");
				tag_t tagObject = NULLTAG;
				try {
					EZXITK_POM_uid_to_tag(uid, tagObject);
					bool isValid = false;
					EZXITK_POM_is_tag_valid(tagObject, isValid);
					if (!isValid) {
						EzMessageManager::getInstance().push(EzMessageManager::MESSAGE_SEVERITY_ERROR, EZX_IMPORT_UID_NOT_VALID_OBJECT, uid);
						throw EzInvalidArgumentException(__FILE_LINE__, EZX_IMPORT_UID_NOT_VALID_OBJECT);
					}
				}
				catch (EzBaseException& ex) {
					const char* exception_msg = ex.askCode() ? ex.askExceptionMessage().c_str() : "Unknown Exception occurred.";
					logger.error(__FILE_LINE__, boost::format("%s") % exception_msg);
					throw;
				}
				return EzTCObject(tagObject);
			}

			bool EzTCObject::isCheckedOut() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");

				bool isCheckedOut = false;
				EZXITK_RES_is_checked_out(m_tag, isCheckedOut);
				return isCheckedOut;
			}

			void EzTCObject::checkOut(const string& reason, const string& change_id) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				if (isCheckedOut()) {
					EzMessageManager::getInstance().push(EzMessageManager::MESSAGE_SEVERITY_ERROR, EZX_OBJECT_IS_ALREADY_CHECKOUT, toString());
					throw EzInvalidStateException(__FILE_LINE__, EZX_OBJECT_IS_ALREADY_CHECKOUT);
				}
				EZXITK_RES_checkout(m_tag, reason, change_id, "", RES_EXCLUSIVE_RESERVE);
			}

			void EzTCObject::checkIn() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				if (!isCheckedOut()) {
					EzMessageManager::getInstance().push(EzMessageManager::MESSAGE_SEVERITY_ERROR, EZX_OBJECT_IS_ALREADY_CHECKIN, toString());
					throw EzInvalidStateException(__FILE_LINE__, EZX_OBJECT_IS_ALREADY_CHECKIN);
				}
				EZXITK_RES_checkin(m_tag);
			}

			bool EzTCObject::isValid() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");

				bool isValid = false;
				EZXITK_POM_is_tag_valid(m_tag, isValid);
				return isValid;
			}

			vector<EzTCObject> EzTCObject::getWhereReferenced(int searchscope) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkArgument(__FILE_LINE__, (searchscope >= 9001 && searchscope <= 9003),
					boost::format("Invalid search scope %d is specified.") % searchscope);

				vector<tag_t> refs;
				vector<int> refsLvl;
				vector<int> refsWhereFound;
				vector<tag_t> classes;
				vector<int> classesLvl;
				vector<int> classesWhereFound;

				EZXITK_POM_referencers_of_instance(m_tag, 1, searchscope, refs, refsLvl, refsWhereFound, classes, classesLvl, classesWhereFound);

				vector<EzTCObject> referenceObjects;
				BOOST_FOREACH(tag_t refObject, refs) {
					referenceObjects.push_back(refObject);
				}

				return referenceObjects;
			}

			bool EzTCObject::isStub(const tag_t& object)
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkNotNull(__FILE_LINE__, object, "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");
				std::vector<tag_t> instances;
				instances.push_back(object);
				std::vector<tag_t> loadedInstances;
				bool isStub = false;
				try {
					EZXITK_POM_load_instances_possible(instances, loadedInstances);
				}
				catch (EzTCException& ex) {
					// This is specific error code to capture and check for STUB objects
					// It is the only official way to check if a tag is STUB and also documented in POM_instance_exists ITK API
					if (ex.askCode() == POM_inst_not_local) {
						isStub = true;
					}
					else {
						throw;
					}
				}
				return isStub;
			}

			void EzTCObject::getRelatedSecondaryObjectsAndRelation(vector<EzTCObject> &secondaryObject, vector<EzTCRelation>& tcRelations, vector<tag_t> &nonPriviledgeObjects,
				const string& relationType, const string& secondaryType) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				try {
					tag_t relationTypeTag = NULLTAG;
					if (!relationType.empty()) {
						try {
							EZXITK_GRM_find_relation_type(relationType, relationTypeTag);
						}
						catch (EzTCException& ex) {
							throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: relationType " + relationType + " not found");
						}
					}

					if (!secondaryType.empty()) {
						if (!EzTCTypeAnalyser::isTypeExists(secondaryType)) {
							throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: secondaryType " + secondaryType + " not found");
						}
					}
					vector<tag_t> secondaryObjectTags;
					vector<tag_t> relationObjectTags;

					EZXITK_GRM_list_secondary_objects_and_relation(m_tag, relationTypeTag, secondaryObjectTags, relationObjectTags);
					// check privleged object of respective secondary objects if type mentioned.
					if (secondaryType.empty()) {
						secondaryObject = EzTCAdapter::adaptCollection<tag_t, EzTCObject>(secondaryObjectTags);
						tcRelations = EzTCAdapter::adaptCollection<tag_t, EzTCRelation>(relationObjectTags);
					}
					else {
						std::vector<tag_t> priviledgeObjects;
						std::vector<tag_t> relObjects;
						std::vector<logical> decisions;
						if (secondaryObjectTags.size()) {
							EZXITK_AM_check_privilege_in_bulk(secondaryObjectTags, PRIVILEGE_READ, false, decisions);
						}
						for (int index = 0; index < decisions.size(); index++)
						{
							if (decisions[index])
							{
								priviledgeObjects.push_back(secondaryObjectTags[index]);
								relObjects.push_back(relationObjectTags[index]);
							}
							else {
								nonPriviledgeObjects.push_back(secondaryObjectTags[index]);
								logger.error(__FILE_LINE__, boost::format("\n %s object has no read access. \n.") % secondaryObjectTags[index]);
							}
						}

						for (int i = 0; i < priviledgeObjects.size(); i++) {
							EzTCObject secPriviledgedObject(priviledgeObjects[i]);
							if (secPriviledgedObject.isOfType(secondaryType)) {
								secondaryObject.push_back(secPriviledgedObject);
								tcRelations.push_back(EzTCRelation(relObjects[i]));
							}
						}
					}


				}
				catch (EzBaseException& ex) {
					const char* exception_msg = ex.askCode() ? ex.askExceptionMessage().c_str() : "Unknown Exception occurred.";
					logger.error(__FILE_LINE__, boost::format("%s") % exception_msg);
					throw;
				}

			}
			void EzTCObject::getRelatedPrimaryObjectsAndRelation(vector<EzTCObject> &primaryObject, vector<EzTCRelation>& tcRelations, vector<tag_t> &nonPriviledgeObjects,
				const std::string& relationObjectType, const std::string& primaryObjectType) const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				try {

					tag_t relationTypeTag = NULLTAG;
					if (!relationObjectType.empty()) {
						try {
							EZXITK_GRM_find_relation_type(relationObjectType, relationTypeTag);
						}
						catch (EzTCException& ex) {
							throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: relationType " + relationObjectType + " not found");
						}
					}
					if (!primaryObjectType.empty()) {
						if (!EzTCTypeAnalyser::isTypeExists(primaryObjectType)) {
							throw EzInvalidArgumentException(__FILE_LINE__, "Invalid argument: secondaryType " + primaryObjectType + " not found");
						}
					}
					vector<tag_t> primaryObjectTags;
					vector<tag_t> relationObjectTags;

					EZXITK_GRM_list_primary_objects_and_relation(m_tag, relationTypeTag, primaryObjectTags, relationObjectTags);
					if (primaryObjectType.empty()) {
						primaryObject = EzTCAdapter::adaptCollection<tag_t, EzTCObject>(primaryObjectTags);
						tcRelations = EzTCAdapter::adaptCollection<tag_t, EzTCRelation>(relationObjectTags);

					}
					else {
						std::vector<tag_t> priviledgeObjects;
						std::vector<tag_t> relObjects;

						std::vector<logical> decisions;
						if (primaryObjectTags.size()) {
							EZXITK_AM_check_privilege_in_bulk(primaryObjectTags, PRIVILEGE_READ, false, decisions);
						}
						for (int index = 0; index < decisions.size(); index++)
						{
							if (decisions[index])
							{
								priviledgeObjects.push_back(primaryObjectTags[index]);
								relObjects.push_back(relationObjectTags[index]);
							}
							else {
								nonPriviledgeObjects.push_back(primaryObjectTags[index]);
								logger.error(__FILE_LINE__, boost::format("\n %s object has no read access. \n.") % primaryObjectTags[index]);
							}
						}

						for (int i = 0; i < priviledgeObjects.size(); i++) {
							EzTCObject primaryPriviledgeObj(priviledgeObjects[i]);
							if (primaryPriviledgeObj.isOfType(primaryObjectType)) {
								primaryObject.push_back(primaryPriviledgeObj);
								tcRelations.push_back(EzTCRelation(relObjects[i]));
							}
						}
					}

				}
				catch (EzBaseException& ex) {
					const char* exception_msg = ex.askCode() ? ex.askExceptionMessage().c_str() : "Unknown Exception occurred.";
					logger.error(__FILE_LINE__, boost::format("%s") % exception_msg);
					throw;
				}
			}

			bool EzTCObject::exists() const
			{
				ezx::logger::EzTCLogger logger(loggerModule, __FUNCTION__);
				EzTCPreconditions::checkState(__FILE_LINE__, (m_tag != NULLTAG), "Method call '" + string(__FUNCTION__) + "' on NULLTAG object not allowed.");

				bool isExists = false;
				EZXITK_POM_instance_exists(m_tag, isExists);
				return isExists;
			}

		}
	}
}
