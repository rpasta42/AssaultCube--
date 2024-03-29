enum                            // block types, order matters!
{
    SOLID = 0,                  // entirely solid cube [only specifies wtex]
    CORNER,                     // half full corner of a wall
    FHF,                        // floor heightfield using neighbour vdelta values
    CHF,                        // idem ceiling
    SPACE,                      // entirely empty cube
    SEMISOLID,                  // generated by mipmapping
    MAXTYPE
};

struct sqr
{
    uchar type;                 // one of the above
    char floor, ceil;           // height, in cubes
    uchar wtex, ftex, ctex;     // wall/floor/ceil texture ids
    uchar r, g, b;              // light value at upper left vertex
    uchar vdelta;               // vertex delta, used for heightfield cubes
    char defer;                 // used in mipmapping, when true this cube is not a perfect mip
    char occluded;              // true when occluded
    uchar utex;                 // upper wall tex id
    uchar tag;                  // used by triggers
    uchar reserved[2];
};

enum                            // hardcoded texture numbers
{
    DEFAULT_SKY = 0,
    DEFAULT_LIQUID,
    DEFAULT_WALL,
    DEFAULT_FLOOR,
    DEFAULT_CEIL
};

#define MAPVERSION 8            // bump if map format changes, see worldio.cpp

struct header                   // map file format header
{
    char head[4];               // "CUBE"
    int version;                // any >8bit quantity is little endian
    int headersize;             // sizeof(header)
    int sfactor;                // in bits
    int numents;
    char maptitle[128];
    uchar texlists[3][256];
    int waterlevel;
    uchar watercolor[4];
    int maprevision;
    int ambient;
    int reserved[12];
    char mediareq[128];         // new since version 7 (flowtron) // actually a maximum of 124 will ever be used (24*5+4)
};

struct mapstats
{
    struct header hdr;
    int entcnt[MAXENTTYPES];
    int cgzsize;
    uchar *enttypes;
    short *entposs;
    int spawns[3];
    int flags[2];
    int flagents[2];
    bool hasffaspawns;
    bool hasteamspawns;
    bool hasflags;
};

#define TRANSFORMOLDENTITIES(headr) \
        if(e.type==LIGHT) \
        { \
            if(!e.attr2) e.attr2 = 255; /* needed for MAPVERSION<=2 */ \
            if(e.attr1>32) e.attr1 = 32; /* 12_03 and below */ \
        } \
        if(headr.version<6  && strncmp(headr.head,"CUBE",4)==0)  /* only render lights, pl starts and map models on old maps // <6 was <MAPVERSION but we're now at #7 (flowtron) */ \
        { \
            switch(e.type) \
            { \
                case 1: /* old light */ \
                    e.type=LIGHT; \
                    break; \
                case 2: /* old player start */ \
                    e.type=PLAYERSTART; \
                    break; \
                case 3: \
                case 4: \
                case 5: \
                case 6: \
                    e.type=I_AMMO; \
                    break; \
                case 7: /* old health */ \
                    e.type=I_HEALTH; \
                    break; \
                case 8: /* old boost */ \
                    e.type=I_HEALTH; \
                    break; \
                case 9: /* armour */ \
                    e.type=I_HELMET; \
                    break; \
                case 10: /* armor */ \
                    e.type=I_ARMOUR; \
                    break; \
                case 11: /* quad */ \
                    e.type=I_AKIMBO; \
                    break; \
                case 14: /* old map model */ \
                    e.type=MAPMODEL; \
                    break; \
                default: \
                    e.type=NOTUSED; \
            } \
        } \
        if(headr.version>=6 && headr.version<8) { \
            if( e.type >= I_HELMET && e.type < (MAXENTTYPES - 1) ) { e.type += 1; } \
        }

#define SWS(w,x,y,s) (&(w)[((y)<<(s))+(x)])
#define SW(w,x,y) SWS(w,x,y,sfactor)
#define S(x,y) SW(world,x,y)            // convenient lookup of a lowest mip cube
#define SMALLEST_FACTOR 6               // determines number of mips there can be
#define DEFAULT_FACTOR 8
#define LARGEST_FACTOR 11               // 10 is already insane
#define MAXENTITIES 65535
#define SOLID(x) ((x)->type==SOLID)
#define MINBORD 2                       // 2 cubes from the edge of the world are always solid
#define OUTBORD(x,y) ((x)<MINBORD || (y)<MINBORD || (x)>=ssize-MINBORD || (y)>=ssize-MINBORD)
#define OUTBORDRAD(x,y,rad) (int(x-rad)<MINBORD || int(y-rad)<MINBORD || int(x+rad)>=ssize-MINBORD || (y+rad)>=ssize-MINBORD)
#define MAXMHEIGHT 30
#define MAXMAREA 10000
#define MAXHHITS 15000                  // was 6000, which denied my fav. maps - jamz, 2012-06-12
#define MINFF 2500

struct block { int x, y, xs, ys, h; };

// vertex array format

struct vertex { float u, v, x, y, z; uchar r, g, b, a; };

