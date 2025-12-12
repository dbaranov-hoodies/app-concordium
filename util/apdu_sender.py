from ledgerblue.commTCP import Transport

# Nano S/X using HID interface
transport = Transport(interface="hid", debug=True)
# or Speculos through TCP socket
transport = Transport(interface="tcp", server="127.0.0.1", port=9999, debug=True)

#
# send/recv APDUs
#

# send method for structured APDUs
transport.send(cla=0xe0, ins=0x03, p1=0, p2=0, cdata=b"")  # send b"\xe0\x03\x00\x00\x00"
# or send_raw method for hexadecimal string
transport.send_raw("E003000000")  # send b"\xe0\x03\x00\x00\x00"
# or with bytes type
transport.send_raw(b"\xe0\x03\x00\x00\x00")

# Waiting for a response (blocking IO)
sw, response = transport.recv()  # type: int, bytes

#
# exchange APDUs (one time send/recv)
#

# exchange method for structured APDUs
sw, response = transport.exchange(cla=0xe0, ins=0x03, p1=0, p2=0, cdata=b"")  # send b"\xe0\x03\x00\x00\x00"
# or exchange_raw method for hexadecimal string
sw, response = transport.exchange_raw("E003000000")  # send b"\xe0\x03\x00\x00\x00"
# or with bytes type
sw, response = transport.exchange_raw(b"\xe0\x03\x00\x00\x00")