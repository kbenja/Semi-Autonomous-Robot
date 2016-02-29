## Set up

On Edison, run following commands:

```
echo "src mraa-upm http://iotdk.intel.com/repos/3.0/intelgalactic/opkg/i586" > /etc/opkg/mraa-upm.conf
opkg update
opkg install mraa
cp mraa_adv_func.h /usr/include/mraa
cp mraa_internal_types.h /urs/include/mraa
```

On computer, make sure the two header files are included (I haven't tested this yet)

-DQG

