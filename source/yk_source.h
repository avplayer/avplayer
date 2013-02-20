//
// yk_source.h
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2013 InvXp (invidentxp@gmail.com)
//

#ifndef __YK_SOURCE_H__
#define __YK_SOURCE_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/thread/mutex.hpp>

#include <stdint.h>
#include "av_source.h"

#include <string>
#include <vector>

#include "libyk.h"

using namespace libyk;
// yk�е���Ƶ��Ϣ.
struct yk_video_file_info 
{
    int index;              // id.
    std::string title;      // ��Ƶ�ļ���.
    std::string source;     // ��ƵԴ��ַ
    uint64_t data_size;    // ��Ƶ��С.
    uint64_t base_offset;  // ��Ƶ��yk�е�ƫ��.
    int status;             // ��ǰ����״̬.
};

class yk_source
    : public av_source
{
public:
    yk_source();
    virtual ~yk_source();

public:
    // ��.
    virtual bool open(void* ctx);

    // ��ȡ����.
    virtual bool read_data(char* data, uint64_t offset, size_t size, size_t& read_size);

    // seek����, �˴�����true, ��ʾ���ݲ���, ��Ҫ����.
    virtual bool read_seek(uint64_t offset, int whence);

    // �ر�.
    virtual void close();

    // ���û��õ�ǰ���ŵ���Ƶ�ļ�.
    virtual bool set_current_video(int index);
    virtual bool get_current_video(yk_video_file_info& vfi) const;

    // ��ǰ��Ƶ�б�.
    virtual std::vector<yk_video_file_info> video_list() const { return m_videos; }

    // ���ö�ȡ����.
    virtual void reset();
    
public:
    libykvideo                      m_yk_video;
    std::vector<yk_video_file_info> m_videos;
 
private:

    // ��ǰ���ŵ���Ƶ.
    yk_video_file_info m_current_video;

    // yk��������Ƶ��Ϣ.

    // ���ڿ������ö�ȡ����.
    bool m_reset;

    // �رտ���.
    bool m_abort;
    boost::mutex m_abort_mutex;
};

#endif // __YK_SOURCE_H__

