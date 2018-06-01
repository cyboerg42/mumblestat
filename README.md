# mumblestat

you want to check mumble from a cli? now you can!

## usage
```
make
sudo make install
mstat IP PORT INTERVAL
```
## conclusion

i should really take a look at PROTOBUF. it would make my life way easier :)

## get data into influxdb
```
mkdir /root/log_scripts/
cp ./influx.sh /root/log_scripts/influx.sh
cp ./mstat /root/log_scripts/mstat
crontab -e

* * * * * /bin/bash /root/log_scripts/influx.sh 213.239.213.45
* * * * * ( sleep 30 ; /bin/bash /root/log_scripts/influx.sh 213.239.213.45)
```
## get data from influxdb
```
SELECT mean("value") FROM "mumblestat.ping" WHERE ("host" = '213.239.213.45') AND $timeFilter GROUP BY time($__interval) fill(none)

SELECT mean("value") FROM "mumblestat.users" WHERE ("host" = '213.239.213.45') AND $timeFilter GROUP BY time($__interval) fill(none)
```
