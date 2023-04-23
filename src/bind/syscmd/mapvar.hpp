#ifndef _MAPVAR_HPP
#define _MAPVAR_HPP

#include "map.hpp"


namespace vind
{
    namespace bind
    {
        // General
        struct Map : public MapBase<Map> {
            explicit Map()
            : MapBase("map")
            {}
        } ;
        struct Noremap : public NoremapBase<Noremap> {
            explicit Noremap()
            : NoremapBase("noremap")
            {}
        } ;
        struct Unmap : public MapBase<Unmap> {
            explicit Unmap()
            : MapBase("unmap")
            {}
        } ;
        struct Mapclear : public MapBase<Mapclear> {
            explicit Mapclear()
            : MapBase("mapclear")
            {}
        } ;

        // GUI
        struct GMap : public MapBase<GMap> {
            explicit GMap()
            : MapBase("gmap")
            {}
        } ;
        struct GNoremap : public NoremapBase<GNoremap> {
            explicit GNoremap()
            : NoremapBase("gnoremap")
            {}
        } ;
        struct GUnmap : public MapBase<GUnmap> {
            explicit GUnmap()
            : MapBase("gunmap")
            {}
        } ;
        struct GMapclear : public MapBase<GMapclear> {
            explicit GMapclear()
            : MapBase("gmapclear")
            {}
        } ;

        // Editor
        struct EMap : public MapBase<EMap> {
            explicit EMap()
            : MapBase("emap")
            {}
        } ;
        struct ENoremap : public NoremapBase<ENoremap> {
            explicit ENoremap()
            : NoremapBase("enoremap")
            {}
        } ;
        struct EUnmap : public MapBase<EUnmap> {
            explicit EUnmap()
            : MapBase("eunmap")
            {}
        } ;
        struct EMapclear : public MapBase<EMapclear> {
            explicit EMapclear()
            : MapBase("emapclear")
            {}
        } ;

        // Normal
        struct NMap : public MapBase<NMap> {
            explicit NMap()
            : MapBase("nmap")
            {}
        } ;
        struct NNoremap : public NoremapBase<NNoremap> {
            explicit NNoremap()
            : NoremapBase("nnoremap")
            {}
        } ;
        struct NUnmap : public MapBase<NUnmap> {
            explicit NUnmap()
            : MapBase("nunmap")
            {}
        } ;
        struct NMapclear : public MapBase<NMapclear> {
            explicit NMapclear()
            : MapBase("nmapclear")
            {}
        } ;

        // Visual
        struct VMap : public MapBase<VMap> {
            explicit VMap()
            : MapBase("vmap")
            {}
        } ;
        struct VNoremap : public NoremapBase<VNoremap> {
            explicit VNoremap()
            : NoremapBase("vnoremap")
            {}
        } ;
        struct VUnmap : public MapBase<VUnmap> {
            explicit VUnmap()
            : MapBase("vunmap")
            {}
        } ;
        struct VMapclear : public MapBase<VMapclear> {
            explicit VMapclear()
            : MapBase("vmapclear")
            {}
        } ;

        // GUI Normal
        struct GNMap : public MapBase<GNMap> {
            explicit GNMap()
            : MapBase("gnmap")
            {}
        } ;
        struct GNNoremap : public NoremapBase<GNNoremap> {
            explicit GNNoremap()
            : NoremapBase("gnnoremap")
            {}
        } ;
        struct GNUnmap : public MapBase<GNUnmap> {
            explicit GNUnmap()
            : MapBase("gnunmap")
            {}
        } ;
        struct GNMapclear : public MapBase<GNMapclear> {
            explicit GNMapclear()
            : MapBase("gnmapclear")
            {}
        } ;

        // GUI Visual
        struct GVMap : public MapBase<GVMap> {
            explicit GVMap()
            : MapBase("gvmap")
            {}
        } ;
        struct GVNoremap : public NoremapBase<GVNoremap> {
            explicit GVNoremap()
            : NoremapBase("gvnoremap")
            {}
        } ;
        struct GVUnmap : public MapBase<GVUnmap> {
            explicit GVUnmap()
            : MapBase("gvunmap")
            {}
        } ;
        struct GVMapclear : public MapBase<GVMapclear> {
            explicit GVMapclear()
            : MapBase("gvmapclear")
            {}
        } ;

        // EDI Normal
        struct ENMap : public MapBase<ENMap> {
            explicit ENMap()
            : MapBase("enmap")
            {}
        } ;
        struct ENNoremap : public NoremapBase<ENNoremap> {
            explicit ENNoremap()
            : NoremapBase("ennoremap")
            {}
        } ;
        struct ENUnmap : public MapBase<ENUnmap> {
            explicit ENUnmap()
            : MapBase("enunmap")
            {}
        } ;
        struct ENMapclear : public MapBase<ENMapclear> {
            explicit ENMapclear()
            : MapBase("enmapclear")
            {}
        } ;

        // EDI Visual
        struct EVMap : public MapBase<EVMap> {
            explicit EVMap()
            : MapBase("evmap")
            {}
        } ;
        struct EVNoremap : public NoremapBase<EVNoremap> {
            explicit EVNoremap()
            : NoremapBase("evnoremap")
            {}
        } ;
        struct EVUnmap : public MapBase<EVUnmap> {
            explicit EVUnmap()
            : MapBase("evunmap")
            {}
        } ;
        struct EVMapclear : public MapBase<EVMapclear> {
            explicit EVMapclear()
            : MapBase("evmapclear")
            {}
        } ;

        // Insert
        struct IMap : public MapBase<IMap> {
            explicit IMap()
            : MapBase("imap")
            {}
        } ;
        struct INoremap : public NoremapBase<INoremap> {
            explicit INoremap()
            : NoremapBase("inoremap")
            {}
        } ;
        struct IUnmap : public MapBase<IUnmap> {
            explicit IUnmap()
            : MapBase("iunmap")
            {}
        } ;
        struct IMapclear : public MapBase<IMapclear> {
            explicit IMapclear()
            : MapBase("imapclear")
            {}
        } ;

        // Resident
        struct RMap : public MapBase<RMap> {
            explicit RMap()
            : MapBase("rmap")
            {}
        } ;
        struct RNoremap : public NoremapBase<RNoremap> {
            explicit RNoremap()
            : NoremapBase("rnoremap")
            {}
        } ;
        struct RUnmap : public MapBase<RUnmap> {
            explicit RUnmap()
            : MapBase("runmap")
            {}
        } ;
        struct RMapclear : public MapBase<RMapclear> {
            explicit RMapclear()
            : MapBase("rmapclear")
            {}
        } ;

        // Command
        struct CMap : public MapBase<CMap> {
            explicit CMap()
            : MapBase("cmap")
            {}
        } ;
        struct CNoremap : public NoremapBase<CNoremap> {
            explicit CNoremap()
            : NoremapBase("cnoremap")
            {}
        } ;
        struct CUnmap : public MapBase<CUnmap> {
            explicit CUnmap()
            : MapBase("cunmap")
            {}
        } ;
        struct CMapclear : public MapBase<CMapclear> {
            explicit CMapclear()
            : MapBase("cmapclear")
            {}
        } ;
    }
}

#endif
