#ifndef HTCW_ZIP_HPP
#define HTCW_ZIP_HPP

#include <io_stream.hpp>

namespace zip {
    typedef size_t(*archive_write_callback)(const uint8_t* data, size_t length, void* state);
    static_assert(bits::endianness()!=bits::endian_mode::none,"Please define HTCW_LITTLE_ENDIAN or HTCW_BIG_ENDIAN before including zip to indicate the byte order of the platform.");
    enum struct zip_result {
        success = 0,
        invalid_argument = 1,
        invalid_archive = 2,
        not_supported = 3,
        io_error = 4,
        invalid_state = 5,
        out_of_memory = 6
    };

    extern zip_result inflate(io::stream* in,archive_write_callback write_callback, void* write_callback_state,long long int in_size=-1,void*(*allocator)(size_t)=malloc,void(*deallocator)(void*)=free);
    class archive;
    class archive_entry;

    class archive_entry final {
        friend class archive;
        long long int m_local_header_offset;
        uint16_t m_compression_method;
        size_t m_compressed_size;
        size_t m_uncompressed_size;
        io::stream* m_stream;
    public:
        archive_entry();
        bool initialized() const;
        size_t copy_path(char* buffer,size_t size) const;
        size_t uncompressed_size() const;
        size_t compressed_size() const;
        zip_result extract(io::stream* out_stream,void*(*allocator)(size_t)=malloc,void(*deallocator)(void*)=free) const;
        zip_result extract(archive_write_callback write_callback, void* write_callback_state = nullptr,void*(*allocator)(size_t)=malloc,void(*deallocator)(void*)=free) const;
    };
    class archive final {
        size_t m_entries_size;
        long long int m_offset;
        io::stream* m_stream;
    
        zip_result init(io::stream *stream);
public:
        archive();
        archive(io::stream* stream);
        static zip_result open(io::stream* stream,archive* out_archive);
        bool initialized() const;
        size_t entries_size() const;
        zip_result entry(size_t index,archive_entry* out_entry) const;
    };
}
#endif