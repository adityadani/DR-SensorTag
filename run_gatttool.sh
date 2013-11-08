#!/usr/bin/expect

set arg1 [lindex $argv 0]

set timeout 5

spawn sudo hcitool lescan
spawn gatttool -b $arg1 -I
expect "$arg1"
send "connect\r"
expect "CON"


# Saving the current spawn id.
set orig $spawn_id

# Get Temperature
send -i $orig "char-write-req 0x29 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-write-req 0x26 0100\r"
expect -i $orig  "written successfully"
send -i $orig "\r\rchar-read-hnd 0x26\r"
expect -i $orig "Characteristic value"

# Get Accelerometer
send -i $orig "char-write-req 0x31 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-write-req 0x2E 0100\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x31\r"
expect -i $orig "Characteristic value"

# Get Humidity
send -i $orig "char-write-req 0x3C 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-write-req 0x39 0100\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x3C\r"
expect -i $orig "Characteristic value"

# Get Magnetometer
send  -i $orig "char-write-req 0x44 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-write-req 0x41 0100\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x44\r"
expect -i $orig "Characteristic value"

# Get Gyroscope
send -i $orig "char-write-req 0x5B 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-write-req 0x58 0100\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x5B\r"
expect -i $orig "Characteristic value"

set count 1
while {$count > 0} {
    

# Get Barometer
send -i $orig "char-write-req 0x4F 02\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x52\r"
expect -i $orig "descriptor: "
expect -i $orig "CON"
set calib_data $expect_out(buffer)
send -i $orig "\r\rchar-write-req 0x4F 01\r"
expect -i $orig "written successfully"
send -i $orig "\r\rchar-read-hnd 0x4B\r"
expect -i $orig "descriptor: "
expect -i $orig "CON"
spawn ls empty
expect eof
exp_wait


set count [expr $count+1];

puts "Counter : $count"
}

puts "Done executing"
send "\r\rquit\r"
expect "optimus"
