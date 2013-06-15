/* simple.c

   Simple libftdi usage example

   This program is distributed under the GPL, version 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>
#include <signal.h>

int quit = 0;	//variable that gets set to 1 when we are ready to exit

void sigintHandler(int signum)
{

    printf("Caught signal %d", signum);
    quit = 1;
}
int main(void)
{
		//set interrupt handler for CTRL+C
    signal(SIGINT,sigintHandler);
    int ret;
    struct ftdi_context *ftdi;
    struct ftdi_version_info version;
    if ((ftdi = ftdi_new()) == 0)
   {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }

    version = ftdi_get_library_version();
    printf("Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
        version.version_str, version.major, version.minor, version.micro,
        version.snapshot_str);

    if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6014)) < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }

    // Read out FTDIChip-ID of R type chips
    if (ftdi->type == TYPE_232H)
    {
        unsigned int chipid;
        printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
        printf("FTDI chipid: %X\n", chipid);
    }
    printf("Resetting...");
    if ((ret = ftdi_set_bitmode(ftdi, 0x00, BITMODE_RESET)) != 0)
    {
        fprintf(stderr, "unable to reset: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }
    printf(" success!\n");

    printf("Setting mode 245...");
    if ((ret = ftdi_set_bitmode(ftdi, 0x00, BITMODE_SYNCFF)) != 0)
    {
        fprintf(stderr, "unable to set to mode 245: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;
    }
    printf(" success!\n");


    //ACBUS5 already outputs the 30MHz clock

    //Is this the bit that caused the error? Try commenting out for now and see what it does.
    /*
    printf("Setting ACBUS[8] to 30 MHz...");
    if ((ret = ftdi_set_eeprom_value(ftdi,CBUS_FUNCTION_8,CBUSH_CLK30)) != 0)
    {

        fprintf(stderr, " unable to set: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        return EXIT_FAILURE;

    }
    else
    {
	printf(" success!\n");
    }
    

       //lets make sure that the eeprom value was set....
    int val;
    ret = ftdi_get_eeprom_value(ftdi,CBUS_FUNCTION_8,&val);
    printf("ret: %d  val: %d\n",ret,val);
    printf("does val match %d?\n",CBUSH_CLK30);
    */
    unsigned char buf[1024];
    int its = 10000;
    return;
    while(quit == 0)
    {

        //ret = ftdi_read_data(&ftdic,&buf,1);
        if ((ret = ftdi_read_data(ftdi,buf,3)) < 0)
        {
            fprintf(stderr, "unable to read ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
            //return EXIT_FAILURE;
        }
	else
	{
	    printf("Read in %x %x %x\n", buf[0],buf[1],buf[2]);
	}
    }
    if ((ret = ftdi_usb_close(ftdi)) < 0)
    {
        fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }

    ftdi_free(ftdi);

    return EXIT_SUCCESS;
}
