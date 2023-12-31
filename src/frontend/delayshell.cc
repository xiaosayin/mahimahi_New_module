/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <vector>
#include <string>

#include "delay_queue.hh"
#include "util.hh"
#include "ezio.hh"
#include "packetshell.cc"

using namespace std;

int main( int argc, char *argv[] )
{
    try {
        const bool passthrough_until_signal = getenv( "MAHIMAHI_PASSTHROUGH_UNTIL_SIGNAL" );

        /* clear environment while running as root */
        char ** const user_environment = environ;
        environ = nullptr;

        check_requirements( argc, argv );

        if ( argc < 2 ) {
            throw runtime_error( "Usage: " + string( argv[ 0 ] ) + " delay-milliseconds [command...]" );
        }

        const uint64_t delay_ms = myatoi( argv[ 1 ] );
        int64_t fluc_ms = 0;
        int arg_next_index = 2;
        if(argc >= 3){
            // judge whether number is valid
            if(argv[2][0] >= '0' && argv[2][0] <= '9'){
                fluc_ms = myatoi(argv[2]);
                arg_next_index = 3;
            }
        }
        // uint64_t delay_ms = myatoi( argv[ 1 ] );

        vector< string > command;
        
        // cout << myatoi( argv[2] ) << endl;

        if ( argc == 2 || argc ==3) {
            command.push_back( shell_path() );
        } else {
            for ( int i = arg_next_index; i < argc; i++ ) {
                command.push_back( argv[ i ] );
            }
        }

        PacketShell<DelayQueue> delay_shell_app( "delay", user_environment, passthrough_until_signal );
        
        delay_shell_app.start_uplink( "[delay " + to_string( delay_ms ) + " ms] ",
                                      command,
                                      delay_ms, fluc_ms);
        delay_shell_app.start_downlink( delay_ms, fluc_ms);
        return delay_shell_app.wait_for_exit();
    } catch ( const exception & e ) {
        print_exception( e );
        return EXIT_FAILURE;
    }
}
