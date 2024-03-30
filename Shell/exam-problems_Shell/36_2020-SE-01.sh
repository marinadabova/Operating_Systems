#Зад. 36 2020-SE-01 Напишете shell скрипт, който получава два задължителни позиционни параметъра - име на файл (bar.csv) и име на директория.
#Директорията може да съдържа текстови файлове с имена от вида foobar.log, всеки от които има съдържание от следния вид:
#Пример 1 (loz-gw.log):
#Licensed features for this platform:
#Maximum Physical Interfaces : 8
#VLANs : 20
#Inside Hosts : Unlimited
#Failover : Active/Standby
#VPN-3DES-AES : Enabled
#*Total VPN Peers : 25
#VLAN Trunk Ports : 8
#This platform has an ASA 5505 Security Plus license.
#Serial Number: JMX00000000
#Running Activation Key: 0e268e0c
#Пример 2 (border-lozenets.log):
#Licensed features for this platform:
#Maximum Physical Interfaces : 4
#VLANs : 16
#Inside Hosts : Unlimited
#Failover : Active/Active
#VPN-3DES-AES : Disabled
#*Total VPN Peers : 16
#VLAN Trunk Ports : 4
#This platform has a PIX 535 license.
#Serial Number: PIX5350007
#Running Activation Key: 0xd11b3d48
#Имената на лог файловете (loz-gw, border-lozenets) определят даден hostname, а съдържанието им дава детайли за определени параметри на съответният хост.
#Файлът bar.csv, който трябва да се генерира от вашия скрипт, е т.н. CSV (comma separated values) файл, тоест текстови файл - таблица,
#на който полетата на всеки ред са разделени със запетая. Първият ред се ползва за определяне на имената на колоните.
#Скриптът трябва да създава файла bar.csv на база на log файловете в директорията. Генерираният CSV файл от директория,
#която съдържа само loz-gw.log и border-lozenets.log би изглеждал така:
#hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key
#loz-gw,8,20,Unlimited,Active/Standby,Enabled,25,8,ASA 5505 Security Plus,JMX00000000,0e268e0c
#border-lozenets,4,16,Unlimited,Active/Active,Disabled,16,4,PIX 535,PIX5350007,0xd11b3d48
#Полетата в генерирания от скрипта CSV файл не трябва да съдържат излишни trailing/leading интервали.
#За улеснение, приемете, че всички whitespace символи във входните файлове са символа “интервал”.

#!/bin/bash


if [[ $# -ne 2 ]]; then
        echo "Argumenst should be two"
        exit 1
fi
if [[ ! -d $2 ]]; then
        echo "Second argument should be dir"
        exit 3
fi
touch ./$1
if [[ ! -f $1 ]]; then
     echo "First argument should be file"
     exit 2
fi
if [[ -n $(find $2 -maxdepth 0 -empty) ]]; then
	echo "Directory empty nothing to search for"
	exit 5
fi

if [[ -z $(find $2 -type f -name "*.log") ]]; then
	echo "There are not .log files to read from"
	exit 6
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" >> $1

while read file; do
        hostname=$(basename $file  .log)
        phy=$(cat $file |grep "Maximum"| cut -d ':' -f 2| tr -d ' ')
        vlans=$(cat $file | grep "VLANs" |cut -d ':' -f 2| tr -d ' ')
        hosts=$(cat $file | grep "Inside" |cut -d ':' -f 2| tr -d ' ')
        failover=$(cat $file | grep "Failover" |cut -d ':' -f 2| tr -d ' ')
        vpn_3des=$(cat $file | grep "VPN-3DES_AES" |cut -d ':' -f 2| tr -d ' ')
        peers=$(cat $file | grep "Peers" |cut -d ':' -f 2| tr -d ' ')
        ports=$(cat $file | grep "Ports" |cut -d ':' -f 2| tr -d ' ')
        license=$(cat $file | grep "license"| sed -E 's/(^.*a[n]?[[:space:]]+)(.*)([[:space:]]+license.$)/\2/g')
        sn=$(cat $file | grep "Number" | cut -d ':' -f 2| tr -d ' ')
        key=$(cat $file | grep "Key"| cut -d ':' -f 2| tr -d ' ')
        #read phy vlans hosts failover vpn peers vlan SN key < <(cat $line | awk '$1=="Maximum"{phy=$5}
                #$1=="VLANs" {vlans=$3}
                #$1=="Inside" {hosts=$4}
                #$1=="Failover" {failover=$3}
                #$1=="VPN-3DES-AES" {vpn=$3}
                #$1=="*Total" {peers=$5}
                #$1=="VLAN" {vlan=$5}
                #$1=="Serial" {sn=$3}
                #$1=="Running" {key=$4; print phy,vlans,hosts,failover,vpn,peers,vlan,sn,key}')
echo "$hostname,$phy,$vlans,$hosts,$failover,$vpn_3des,$peers,$ports,$license,$sn,$key" >>$1

done< <(find $2 -type f -regextype egrep -regex ".*\.log$")
