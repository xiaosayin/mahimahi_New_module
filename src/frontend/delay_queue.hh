/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#ifndef DELAY_QUEUE_HH
#define DELAY_QUEUE_HH

#include <queue>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <limits>

#include "file_descriptor.hh"

class DelayQueue
{
private:
    uint64_t delay_ms_;
    int64_t fluc_ms_;
    std::queue< std::pair<uint64_t, std::string> > packet_queue_;
    /* release timestamp, contents */

public:
    DelayQueue( const uint64_t & s_delay_ms ) : delay_ms_( s_delay_ms ), fluc_ms_(0), packet_queue_() {}

    DelayQueue( const uint64_t & s_delay_ms, const int64_t & fluc_ms) : delay_ms_( s_delay_ms ), fluc_ms_(fluc_ms), packet_queue_()
    {   
        int64_t y = static_cast<int64_t>(s_delay_ms > std::numeric_limits<int64_t>::max() ? std::numeric_limits<int64_t>::max() : s_delay_ms);
        if(fluc_ms > y){
            throw std::runtime_error( "Usage: mm-delay delay_ms_ fluc_ms_ and fluc_ms_ must <= delay_ms_" );
        }
        
    }

    void read_packet( const std::string & contents );

    void write_packets( FileDescriptor & fd );

    unsigned int wait_time( void ) const;

    bool pending_output( void ) const { return wait_time() <= 0; }

    static bool finished( void ) { return false; }
};

#endif /* DELAY_QUEUE_HH */
