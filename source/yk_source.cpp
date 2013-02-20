#include "yk_source.h"

#ifdef USE_YK
#include <fstream>
#include <boost/filesystem.hpp>


yk_source::yk_source()
: m_abort(false)
, m_reset(false)
{
    m_current_video.index = -1;
}

yk_source::~yk_source()
{
    close();
}

bool yk_source::open(void* ctx)
{
    if (!ctx)
        return false;

    m_abort = false;

    return true;
}

bool yk_source::read_data(char* data, uint64_t offset, size_t size, size_t& read_size)
{
    if ( !data || m_videos.empty() )
        return false;

    bool ret = false;
    int piece_offset = 0;

/*
    // �˴���bug, ������read_data�����˳���, ����destroy.
    read_size = 0;
    m_reset = false;

    // ��ȡ����Խ��.
    if (offset >= m_current_video.data_size)
        return false;

    // ����ƫ��.
    offset += m_current_video.base_offset;
    const torrent_info& info = m_torrent_handle.get_torrent_info();
    piece_offset = offset / info.piece_length();

    boost::mutex::scoped_lock lock(m_abort_mutex);
    // ��ȡ����.
    try
    {
        while (!m_abort && !m_reset)
        {
            boost::int64_t rs = 0;
            ret = m_read_op->read_data(data, offset, size, rs);
            if (ret)
            {
                read_size = rs;
                break;
            }
            else
            {
                boost::this_thread::sleep(boost::posix_time::millisec(100));
            }

            if (m_reset)
                return true;
        }
    }
    catch (boost::thread_interrupted& e)
    {
        printf("read thread is interrupted!\n");
        return false;
    }
    catch (...)
    {
        printf("read thread is interrupted!\n");
        return false;
    }
*/
    return ret;
}

bool yk_source::read_seek(uint64_t offset, int whence)
{
    /*
    if (!m_read_op || m_videos.size() == 0)
        return false;

    if (offset >= m_current_video.data_size)
        return false;

    // ������������seekʱ, ����λ�õķֿ����һ�����Ƿ��Ѿ�����, ���δ����, ��֪ͨ�ϲ������ͣ�����߼�.
    if (whence == SEEK_SET || whence == SEEK_CUR || whence == SEEK_END)
    {
        torrent_status status = m_torrent_handle.status();
        const torrent_info& info = m_torrent_handle.get_torrent_info();
        int piece_length = info.piece_length();
        int piece_index = offset / piece_length;

        if (!status.pieces[piece_index])
            return true;
        if (++piece_index < status.num_pieces)
        {
            if (!status.pieces[piece_index])
                return true;
        }
    }
    */
    return false;
}

void yk_source::close()
{
    m_abort = true;
}

bool yk_source::set_current_video(int index)
{
    // ����Ƿ��ʼ���������Ƿ���Ч.
    if (index >= m_videos.size() || index < 0)
        return false;
    // ����Ϊ��ǰ��Ƶ.
    m_current_video = m_videos[index];

    return true;
}

bool yk_source::get_current_video(yk_video_file_info& vfi) const
{
    // ����Ϊ��ǰ��Ƶ.
    vfi = m_current_video;

    return true;
}

void yk_source::reset()
{
    m_reset = true;
}

#endif // USE_YK
