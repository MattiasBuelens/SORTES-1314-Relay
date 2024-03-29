# SDCC paths
# A) Linux
# LIB = /usr/local/lib
# INCLUDE = /usr/local/share/sdcc/include
# B) Windows
#    Do not use spaces in these paths! (set up symbolic links if needed)
LIB = C:\SDCC\lib
INCLUDE = C:\SDCC\include

AS = gpasm
CC = sdcc
CFLAGS= -c -mpic16 -p18f97j60 -o$@ 
LD = sdcc
LDFLAGS= -mpic16 -p18f97j60 -L$(LIB)/pic16 -llibio18f97j60.lib \
         -llibdev18f97j60.lib -llibc18f.lib
AR = ar
RM = rm

OBJECTS=Objects/main.o Objects/Announce.o Objects/ARP.o Objects/Delay.o \
   Objects/DHCP.o Objects/DHCPs.o Objects/DNS.o  Objects/ETH97J60.o  \
   Objects/Hashes.o Objects/Helpers.o Objects/ICMP.o Objects/IP.o \
   Objects/LCDBlocking.o Objects/StackTsk.o \
   Objects/Tick.o Objects/UDP.o \
   Objects/relay.o Objects/platform_pic16.o

SDCC_HEADERS=$(INCLUDE)/string.h \
   $(INCLUDE)/stdlib.h \
   $(INCLUDE)/stdio.h \
   $(INCLUDE)/stddef.h \
   $(INCLUDE)/stdarg.h 

SDCC_PIC16_HEADERS=$(INCLUDE)/pic16/pic18f97j60.h

TCPIP_HEADERS=Include/TCPIP_Stack/Helpers.h \
   Include/delays.h \
   Include/TCPIP_Stack/Tick.h \
   Include/TCPIP_Stack/MAC.h \
   Include/TCPIP_Stack/IP.h \
   Include/TCPIP_Stack/ARP.h \
   Include/TCPIP_Stack/Hashes.h \
   Include/TCPIP_Stack/XEEPROM.h \
   Include/TCPIP_Stack/UDP.h \
   Include/TCPIP_Stack/TCP.h \
   Include/TCPIP_Stack/BerkeleyAPI.h \
   Include/TCPIP_Stack/UART.h \
   Include/TCPIP_Stack/DHCP.h \
   Include/TCPIP_Stack/DNS.h \
   Include/TCPIP_Stack/DynDNS.h \
   Include/TCPIP_Stack/MPFS.h \
   Include/TCPIP_Stack/MPFS2.h \
   Include/TCPIP_Stack/HTTP2.h \
   Include/TCPIP_Stack/ICMP.h \
   Include/TCPIP_Stack/SNMP.h \
   Include/TCPIP_Stack/DynDNS.h \
   Include/TCPIP_Stack/SMTP.h \
   Include/TCPIP_Stack/TCPIP.h \
   Include/TCPIP_Stack/StackTsk.h \
   Include/TCPIP_Stack/Delay.h \
   Include/TCPIP_Stack/ETH97J60.h \
   Include/TCPIP_Stack/SPIFlash.h \
   Include/TCPIP_Stack/SPIRAM.h \
   Include/TCPIP_Stack/LCDBlocking.h \
   Include/TCPIP_Stack/UART2TCPBridge.h \
   Include/TCPIP_Stack/Announce.h \
   Include/TCPIP_Stack/NBNS.h \
   Include/TCPIP_Stack/Telnet.h \
   Include/TCPIP_Stack/Reboot.h \
   Include/TCPIP_Stack/SNTP.h \
   Include/TCPIP_Stack/TCPPerformanceTest.h \
   Include/TCPIP_Stack/UDPPerformanceTest.h \
   Include/TCPIP_Stack/Helpers.h \
   Include/TCPIP_Stack/BigInt.h \
   Include/TCPIP_Stack/SSL.h \
   Include/TCPIP_Stack/RSA.h \
   Include/TCPIP_Stack/ARCFOUR.h \
   Include/TCPIP_Stack/AutoIP.h \
   Include/TCPIP_Stack/Random.h \
   Include/TCPIP_Stack/FTP.h \
   Include/TCPIP_Stack/HTTP.h \
   Include/TCPIP_Stack/HTTP2.h \
   Include/TCPIP_Stack/SMTP.h \
   Include/TCPIP_Stack/TFTPc.h

APP_HEADERS=Include/GenericTypeDefs.h \
   Include/Compiler.h \
   Include/HardwareProfile.h \
   Include/TCPIPConfig.h \
   Include/mib.h

relay : $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS)

Objects/main.o : main.c $(SDCC_HEADERS) $(SDCC_PIC16_HEADERS) \
   $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) main.c

Objects/relay.o : relay.c $(SDCC_HEADERS) $(SDCC_PIC16_HEADERS) \
   $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) relay.c

Objects/platform_pic16.o : platform_pic16.c $(SDCC_HEADERS) $(SDCC_PIC16_HEADERS) \
   $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) platform_pic16.c

Objects/CustomHTTPApp.o : TCPIP_Stack/CustomHTTPApp.c $(SDCC_HEADERS) \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/CustomHTTPApp.c

Objects/TFTPc.o : TCPIP_Stack/TFTPc.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/TFTPc.c

Objects/Announce.o : TCPIP_Stack/Announce.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Announce.c

Objects/ARP.o : TCPIP_Stack/ARP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ARP.c

Objects/Delay.o : TCPIP_Stack/Delay.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Delay.c

Objects/DHCP.o : TCPIP_Stack/DHCP.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/DHCP.c

Objects/DHCPs.o : TCPIP_Stack/DHCPs.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/DHCPs.c

Objects/DNS.o : TCPIP_Stack/DNS.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/DNS.c

Objects/DynDNS.o : TCPIP_Stack/DynDNS.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/DynDNS.c

Objects/ENC28J60.o : TCPIP_Stack/ENC28J60.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ENC28J60.c

Objects/ETH97J60.o : TCPIP_Stack/ETH97J60.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ETH97J60.c

Objects/FTP.o : TCPIP_Stack/FTP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/FTP.c

Objects/Hashes.o : TCPIP_Stack/Hashes.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Hashes.c

Objects/Helpers.o : TCPIP_Stack/Helpers.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Helpers.c

Objects/HTTP2.o : TCPIP_Stack/HTTP2.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/HTTP2.c

Objects/HTTP.o : TCPIP_Stack/HTTP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/HTTP.c

Objects/ICMP.o : TCPIP_Stack/ICMP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ICMP.c

Objects/IP.o : TCPIP_Stack/IP.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/IP.c

Objects/LCDBlocking.o : TCPIP_Stack/LCDBlocking.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/LCDBlocking.c

Objects/MPFS2.o : TCPIP_Stack/MPFS2.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/MPFS2.c

Objects/MPFS.o : TCPIP_Stack/MPFS.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/MPFS.c

Objects/NBNS.o : TCPIP_Stack/NBNS.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/NBNS.c

Objects/Reboot.o : TCPIP_Stack/Reboot.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Reboot.c

Objects/SMTP.o : TCPIP_Stack/SMTP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SMTP.c

Objects/SNMP.o : TCPIP_Stack/SNMP.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SNMP.c

Objects/SNTP.o : TCPIP_Stack/SNTP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SNTP.c

Objects/SPIEEPROM.o : TCPIP_Stack/SPIEEPROM.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SPIRRPROM.c

Objects/SPIFlash.o : TCPIP_Stack/SPIFlash.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SPIFlash.c

Objects/SPIRAM.o : TCPIP_Stack/SPIRAM.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SPIRAM.c

Objects/StackTsk.o : TCPIP_Stack/StackTsk.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/StackTsk.c

Objects/TCP.o : TCPIP_Stack/TCP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/TCP.c

Objects/TCPPerformanceTest.o : TCPIP_Stack/TCPPerformanceTest.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/TCPPerformanceTest.c

Objects/Telnet.o : TCPIP_Stack/Telnet.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Telnet.c

Objects/UDPPerformanceTest.o : TCPIP_Stack/UDPPerformanceTest.c $(SDCC_HEADERS)\
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/UDPPerformanceTest.c

Objects/Tick.o : TCPIP_Stack/Tick.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Tick.c

Objects/UART2TCPBridge.o : TCPIP_Stack/UART2TCPBridge.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/UART2TCPBridge.c

Objects/UART.o : TCPIP_Stack/UART.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/UART.c

Objects/UDP.o : TCPIP_Stack/UDP.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/UDP.c

Objects/MPFSImg.o : MPFSImg.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/MPFSImg.c

Objects/MPFSImg2.o : MPFSImg2.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/MPFSImg2.c

Objects/LegacyHTTPApp.o : LegacyHTTPApp.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/LegacyHTTPApp.c

Objects/PingDemo.o : PingDemo.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) PingDemo.c

Objects/SMTPDemo.o : SMTPDemo.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) SMTPDemo.c

Objects/UARTConfig.o : UARTConfig.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) UARTConfig.c

Objects/GenericTCPClient.o : GenericTCPClient.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) GenericTCPClient.c

Objects/GenericTCPServer.o : GenericTCPServer.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) GenericTCPServer.c

Objects/CustomSNMPApp.o : CustomSNMPApp.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) CustomSNMPApp.c

Objects/BerkeleyUDPClientDemo.o : BerkeleyUDPClientDemo.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) BerkeleyUDPClientDemo.c

Objects/BerkeleyTCPClientDemo.o : BerkeleyTCPClientDemo.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) BerkeleyTCPClientDemo.c

Objects/BerkeleyTCPServerDemo.o : BerkeleyTCPServerDemo.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) BerkeleyTCPServerDemo.c

Objects/BerkeleyAPI.o : TCPIP_Stack/BerkeleyAPI.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/BerkeleyAPI.c

Objects/ENCX24J600.o : TCPIP_Stack/ENCX24J600.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS) $(TCPIP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ENCX24J600.c

Objects/ARCFOUR.o : TCPIP_Stack/ARCFOUR.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/ARCFOUR.c

Objects/BigInt.o : TCPIP_Stack/BigInt.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/BigInt.c

Objects/Random.o : TCPIP_Stack/Random.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/Random.c

Objects/RSA.o : TCPIP_Stack/RSA.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/RSA.c

Objects/SSL.o : TCPIP_Stack/SSL.c  $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/SSL.c

Objects/CustomSSLCert.o : CustomSSLCert.c $(SDCC_HEADERS)  \
   $(SDCC_PIC16_HEADERS) $(APP_HEADERS)
	$(CC) $(CFLAGS) TCPIP_Stack/CustomSSLCert.c

clean : 
	$(RM) -rf *.hex *.cod *.lst Objects/*.o Objects/*.asm Objects/*.lst
