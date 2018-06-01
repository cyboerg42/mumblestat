#/bin/bash
INFLUX_DB="opentsdb"
INFLUX_HOST="127.0.0.1"
INFLUX_PORT="8086"

mstat=$(./mstat $1 -c 1)

users=$(echo $mstat | cut -f3 -d' ' | cut -f1 -d'/')
ping=$(echo $mstat | cut -f5 -d' ')

curl -s -i -XPOST 'http://$INFLUX_HOST:$INFLUX_PORT/write?db=$INFLUX_DB' --data-binary "mumblestat.ping,host=$1 value=$ping"
curl -s -i -XPOST 'http://$INFLUX_HOST:$INFLUX_PORT/write?db=$INFLUX_DB' --data-binary "mumblestat.users,host=$1 value=$users"



