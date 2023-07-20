/* -*-mode:c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <limits>

#include "delay_queue.hh"
#include "timestamp.hh"
#include <random>

using namespace std;

int64_t generateRandomNumber(int bound) {
    // 创建一个随机数引擎对象
    static random_device rd;
    static mt19937 gen(rd());

    // 创建一个整数分布对象，范围为 [-5, 5]
    static uniform_int_distribution<> dis(-1 * bound, bound);

    // 生成一个随机数，并返回
    return dis(gen);
}

void DelayQueue::read_packet( const string & contents)
{   
    int64_t random_fluc_ms = fluc_ms_;
    if(fluc_ms_ > 0){
        random_fluc_ms = generateRandomNumber(fluc_ms_);
    }
    packet_queue_.emplace( timestamp() + delay_ms_ + random_fluc_ms, contents );
}

void DelayQueue::write_packets( FileDescriptor & fd )
{
    while ( (!packet_queue_.empty())
            && (packet_queue_.front().first <= timestamp()) ) {
        fd.write( packet_queue_.front().second );
        packet_queue_.pop();
    }
}

unsigned int DelayQueue::wait_time( void ) const
{
    if ( packet_queue_.empty() ) {
        return numeric_limits<uint16_t>::max();
    }

    const auto now = timestamp();

    if ( packet_queue_.front().first <= now ) {
        return 0;
    } else {
        return packet_queue_.front().first - now;
    }
}
