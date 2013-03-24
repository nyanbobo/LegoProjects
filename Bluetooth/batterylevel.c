#include <stdio.h>
// Socket, used for Bluetooth socket
#include <sys/socket.h>
#include <sys/types.h>
// Bluetooth headers
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
// Global Variables
int nxtSocket;
int max_message_size = 64;

/****************************************************************
* nxt get battery level
*
 this will get the battery level on the nxt
* returns: the battery level as an integer
***************************************************************/
int nxt_getbattery(void)
{
    /*------------------------------------------------------
* direct command format:
* {length/lsb, length/msb, byte 0, byte 1... byte n}
*
* for getbatterylevel (see direct commands):
* byte 0: 0x00
* byte 1: 0x0b length/lsb: 0x02, the command length is 2 bytes
*
*----------------------------------------------------*/
    char cmd[4]={0x02, 0x00, 0x00, 0x0b};
    char reply[max_message_size];
    int result;
    int blevel;
    int replylength;
    int error = 0;
    //- send request -----------------------------------------
    if ( (result = write(nxtSocket, cmd, 4)) < 0 ) {
        perror("error sending getbatterylevel command ");
        return result;
    }
    //- read reply -----------------------------------------
    // get bluetooth message length
    if ( (result = read(nxtSocket, reply, 2)) < 0) {
        perror("error receiving getbatterylevel reply ");
        return result;
    }
    replylength = reply[0] + (reply[1] * 256);

    printf("Reply message length : %d\n", replylength);

    // get return package
    if ( (result = read(nxtSocket, reply, replylength)) < 0) {
        perror("error receiving getbatterylevel reply ");
        return result;
    }

    // quick check to make sure we got everything
    if ( replylength != result)
    {
        fprintf(stderr, "getbatterylevel : lengths do not match : %d != %d\n",
                replylength, result);
    }

    /*------------------------------------------------------
    * return package format:
    *
     {length/lsb, length/msb, byte0, byte1..., byten}
    * for getbatterylevel:
    * byte0: 0x02
    * byte1: 0x0b
    * byte2: status byte
    * byte3-4: voltage in millivolts (uword)
    * length/lsb: 0x05
    *----------------------------------------------------*/
    // byte 0
    if (reply[0] != 0x02)
    {
        fprintf(stderr, "getbatterylevel : byte 0 : %hhx != 0x02\n", reply[0]);
        error = 1;
    }
    // byte 1
    if (reply[1] != 0x0b)
    {
        fprintf(stderr, "getbatterylevel : byte 1 : %hhx != 0x13\n", reply[1]);
        error = 1;
    }
    // byte 2
    if (reply[2] != 0x00)
    {
        fprintf(stderr, "getbatterylevel : byte 2, status : %hhx \n", reply[2]);
        error = 1;
    }
    if (error)
    {
        return -1;
    }
    // byte 3-4
    blevel = reply[3] + (reply[4] * 255);
    return blevel;
}

int init_bluetooth(char *btAddress) {
    struct sockaddr_rc addr={0};
    int status;
    /*-----------------------------------------------------------
    * SOCK_STREAM
    *
    Provides sequenced, reliable, two-way, connection-based
    *
    byte streams. An out-of-band data transmission
    *
    mechanism may be supported.
    *----------------------------------------------------------*/
    // Allocate a socket
    nxtSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    // Set what type and who to connect to
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba(btAddress, &addr.rc_bdaddr);
    // Connect to the NXT
    status = connect(nxtSocket, (struct sockaddr *)&addr, sizeof(addr) );

    if (status < 0) {
        perror("Error connecting Bluetooth");
        return status;
    }
    return 0;
}

int main (void) {
    // nxt brick alpha bluetooth address
    int blevel;
    char btaddress[18] = "00:16:53:1B:94:C3";
    // initiate bluetooth connection
    if (init_bluetooth(btaddress) < 0)
    {
        close(nxtSocket);
        return 1;
    }
    printf("bluetooth connected to %s \n", btaddress);
    // get battery level (direct command)
    blevel = nxt_getbattery();
    if (blevel < 0) {
        close(nxtSocket);
        return 1;
    }
    printf("battery level: %.2f\n", blevel/100.00);
    close(nxtSocket);
    return 0;
}
