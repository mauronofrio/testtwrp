struct TrueTypeFontKey
{
    int size;
    int dpi;
    char *path;
};

typedef struct
{
    FT_BBox bbox;
    FT_BitmapGlyph glyph;
} TrueTypeCacheEntry;

typedef struct
{
    char *text;
    int max_width;
} StringCacheKey;

struct StringCacheEntry
{
    GGLSurface surface;
    int rendered_bytes; // number of bytes from C string rendered, not number of UTF8 characters!
    StringCacheKey *key;
    struct StringCacheEntry *prev;
    struct StringCacheEntry *next;
};

struct TrueTypeFont
{
    int type;
    int refcount;
    int size;
    int dpi;
    int max_height;
    int base;
    FT_Face face;
    std::map<int, TrueTypeCacheEntry*> glyph_cache;
    std::map<std::string, StringCacheEntry*> string_cache;
    struct StringCacheEntry *string_cache_head;
    struct StringCacheEntry *string_cache_tail;
    pthread_mutex_t mutex;
    TrueTypeFontKey *key;
};

typedef struct StringCacheEntry StringCacheEntry;

typedef struct
{
    FT_Library ft_library;
    std::map<std::string, TrueTypeFont*> fonts;
    pthread_mutex_t mutex;
} FontData;

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

// 32bit FNV-1a hash algorithm
// http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
static const uint32_t FNV_prime = 16777619U;
static const uint32_t offset_basis = 2166136261U;

#define STRING_CACHE_MAX_ENTRIES 400
#define STRING_CACHE_TRUNCATE_ENTRIES 150