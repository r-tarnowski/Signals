#include <iostream>
#include <csignal>
#include <unistd.h>


#include <CommonVals.h>


using std::cout;
using std::endl;

struct sigaction sigIntOrigSigaction;
struct sigaction sigAbrtOrigSigaction;
struct sigaction sigTermOrigSigaction;
struct sigaction sigHupOrigSigaction;

//**************************************************************************
//**************************************************************************
void sigHandler( int sigNum ) {
    //signal( sigNum, sigHandler );
    bool sendSig = false;

    switch ( sigNum ) {

        case SIGABRT:
            cout << "Signal SIGABRT received" << endl;
            cout << "Restoring original signal handler" << endl;
            if ( SUCCESS_RETURN == sigaction( sigNum, & sigAbrtOrigSigaction, nullptr ) ) {
                cout << "Sending the SIGABRT signal to myself" << endl;
                sendSig = true;
            }
            break;

        case SIGINT:
            cout << "Signal SIGINT received" << endl;
            cout << "Restoring original signal handler" << endl;
            if ( SUCCESS_RETURN == sigaction( sigNum, & sigIntOrigSigaction, nullptr ) ) {
                cout << "Sending the SIGINT signal to myself" << endl;
                sendSig = true;
            }
            break;

        case SIGHUP:
            cout << "Signal SIGHUP received" << endl;
            cout << "Restoring original signal handler" << endl;
            if ( SUCCESS_RETURN == sigaction( sigNum, & sigHupOrigSigaction, nullptr ) ) {
                cout << "Sending the SIGHUP signal to myself" << endl;
                sendSig = true;
            }
            break;

        case SIGTERM:
            cout << "Signal SIGTERM received" << endl;
            cout << "Restoring original signal handler" << endl;
            if ( SUCCESS_RETURN == sigaction( sigNum, & sigTermOrigSigaction, nullptr ) ) {
                cout << "Sending the SIGTERM signal to myself" << endl;
                sendSig = true;
            }
            break;


        default:
            cout << "ERROR: unexpected signal received!" << endl;
            break;

    }

    if ( sendSig ) {
        kill( 0, sigNum );
    }
}

void printSigHandlerInfo( int sigNum ) {
    struct sigaction query_action;

    if (sigaction (sigNum, nullptr, &query_action) < 0) {
        cout << "sigaction returns -1 - error occurred" << endl;
    }
    else {
        if ( query_action.sa_handler == SIG_DFL ) {
            cout << "Signal " << sigNum <<  " is handled in the default, fatal manner" << endl;
        }
        else {
            if ( query_action.sa_handler == SIG_IGN ) {
                cout  << "Signal " << sigNum << " is ignored" << endl;
            }
            else {
                cout << "A programmer-defined signal handler is in effect for the signal "
                     << sigNum << endl;
            }
        }
    }
}

//**************************************************************************
//**************************************************************************
int setSigHandlers() {

    struct sigaction newAction;
    newAction.sa_handler = sigHandler;
    sigemptyset( & newAction.sa_mask );
    newAction.sa_flags = 0;

   if ( ERROR_RETURN == sigaction( SIGINT, nullptr, & sigIntOrigSigaction ) ) {
       cout << "setSigHandlers() Can't get signal handler for SIGINT - exiting..." << endl;
       return ERROR_RETURN;
   }
   if ( ERROR_RETURN == sigaction( SIGINT, & newAction, nullptr ) ) {
       cout << "setSigHandlers() Can't set signal handler for SIGINT - exiting..." << endl;
       return ERROR_RETURN;
   }

   if ( ERROR_RETURN == sigaction( SIGABRT, nullptr, & sigAbrtOrigSigaction ) ) {
       cout << "setSigHandlers() Can't get signal handler for SIGABRT - exiting..." << endl;
       return ERROR_RETURN;
   }
   if ( ERROR_RETURN == sigaction( SIGABRT, & newAction, nullptr ) ) {
       cout << "setSigHandlers() Can't set signal handler for SIGABRT - exiting..." << endl;
       return ERROR_RETURN;
   }

   if ( ERROR_RETURN == sigaction( SIGTERM, nullptr, & sigTermOrigSigaction ) ) {
       cout << "setSigHandlers() Can't get signal handler for SIGTERM - exiting..." << endl;
       return ERROR_RETURN;
   }
   if ( ERROR_RETURN == sigaction( SIGTERM, & newAction, nullptr ) ) {
       cout << "setSigHandlers() Can't set signal handler for SIGTERM - exiting..." << endl;
       return ERROR_RETURN;
   }

    if ( ERROR_RETURN == sigaction( SIGHUP, nullptr, & sigHupOrigSigaction ) ) {
       cout << "setSigHandlers() Can't get signal handler for SIGHUP - exiting..." << endl;
       return ERROR_RETURN;
   }
   if ( ERROR_RETURN == sigaction( SIGHUP, & newAction, nullptr ) ) {
       cout << "setSigHandlers() Can't set signal handler for SIGHUP - exiting..." << endl;
       return ERROR_RETURN;
   }

    return SUCCESS_RETURN;
}


void printHeader() {
   cout << endl;
   cout << "===============================================" << endl;
   cout << "My own experiments with signals" << endl;
   cout << "===============================================" << endl;
   cout << endl;
}


int main( int argc, char *argv[] ) {
   printHeader();

   cout << "Status of the SIGINT handler" << endl;
   printSigHandlerInfo( SIGINT );
   cout << "Status of the SIGABRT handler" << endl;
   printSigHandlerInfo( SIGABRT );
   cout << "Status of the SIGTERM handler" << endl;
   printSigHandlerInfo( SIGTERM );
   cout << "Status of the SIGHUP handler" << endl;
   printSigHandlerInfo( SIGHUP );

   if ( ERROR_RETURN == setSigHandlers() ) {
       cout << "Failed to set signal handlers - exiting..." << endl << endl;
   }
   else {
       cout << "Signal handlers successfully set" << endl;
   }


   cout << endl;
   int counter = 0;
   while( true ) {
       printf( "%d\n", counter );
       ++ counter;
       sleep( 1 );
       if ( 10 == counter ) {
           //printf( "\n" );
           counter = 0;
       }
   }

   return 0;
}
