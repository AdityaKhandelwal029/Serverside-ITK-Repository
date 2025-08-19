#!/bin/bash

URL="https://pfirst-ad.tmindia.tatamotors.com/pFirst_SOA/cgi/wsplm.cgi"
ObjectName="$1"
businessUnit="$2"
workFlowName="$3"
targetUserId="$4"
taskName="$5"
filePath="$6"
fileName=$(basename "$filePath")
fileType="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
fileData=$(base64 -w 0 "$filePath")
from="PLM <no-reply@tatamotors.com>"
to="$7"
cc=""
subject="Action Required"
html_content=$(cat <<EOF

EOF
)
payload_file="soap_request.xml"

cat > "$payload_file" <<EOF
<x:Envelope
    xmlns:x="http://schemas.xmlsoap.org/soap/envelope/"
    xmlns:plm="https://pfirst-ad.tmindia.tatamotors.com/PLM/">
    <x:Header/>
    <x:Body>
        <plm:get_Mail_Status>
		    <ObjectName><![CDATA[$ObjectName]]></ObjectName>
		    <businessUnit><![CDATA[$businessUnit]]></businessUnit>
		    <workFlowName><![CDATA[$workFlowName]]></workFlowName>
		    <targetUserId><![CDATA[$targetUserId]]></targetUserId>
		    <taskName><![CDATA[$taskName]]></taskName>
		    <fileName><![CDATA[$fileName]]></fileName>
		    <fileType><![CDATA[$fileType]]></fileType>
		    <fileData>${fileData}</fileData>
		    <from><![CDATA[$from]]></from>
		    <to><![CDATA[$to]]></to>
		    <cc><![CDATA[$cc]]></cc>
		    <subject><![CDATA[$subject]]></subject>
            <html_content><![CDATA[$html_content]]></html_content>
        </plm:get_Mail_Status>
    </x:Body>
</x:Envelope>
EOF
echo "Calling pFirst SOAP webservice..."
response=$(curl -s --ssl-no-revoke -X POST "$URL" -u "plm:plm#321" -H "Content-Type: text/xml; charset=utf-8" --data @"$payload_file")
if echo "$response" | grep -q '<soap:Fault>'; then
  faultstring=$(echo "$response" | tr -d '\n\r' | sed -n 's|.*<faultstring>\(.*\)</faultstring>.*|\1|p')
  echo "SOAP Fault error: $faultstring"
else
  result_json=$(echo "$response" | sed -n 's:.*<result[^>]*>\(.*\)</result>.*:\1:p')
  if [[ -z "$result_json" ]]; then
    echo "Could not extract result from response."
  else
    echo "Result: $result_json"
  fi
fi
rm "$payload_file"
echo
read -p "Press Enter to exit..."
