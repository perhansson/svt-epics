import sys

class FEB:
    def __init__(self,febid,half,hybridlist):
        self.id = febid
        self.half = half
        self.hybrids = hybridlist

            
# mapping between hybrid and FEB IDs
febs= [
    FEB(0,"top",[0,1]),
    FEB(1,"top",[2,3,4,5]),
    FEB(2,"top",[6,7,8,9]),
    FEB(3,"top",[10,11,12,13]),
    FEB(4,"top",[14,15,16,17]),
    FEB(0,"bot",[0,1]),
    FEB(1,"bot",[2,3,4,5]),
    FEB(2,"bot",[6,7,8,9]),
    FEB(3,"bot",[10,11,12,13]),
    FEB(4,"bot",[14,15,16,17])
    ]





def getFebId(half,hyb):
    febid = -1
    for feb in febs:
        if feb.half is half:
            if hyb in feb.hybrids:
                febid = feb.id
    return febid


def buildSvtDaqMap():
    s = """

record(longin, SVT:daq:map:HALF:HYBID) {
    field(VAL,"FEBID")
}

    """
    records = []
    for half in ["bot","top"]:
        for hyb in range(0,18):
            feb = getFebId(half,hyb)
            if feb<0:
                print "Invalid feb id for hyb ", hyb, " and half ", half
                sys.exit(1)
            rec = s
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("HALF",half)            
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records




def buildHybLV():


    s = """


record(sub,SVT:lv:hyb:HALF:HYBID:dvdd:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:avdd:i_rd")
}

record(ai, SVT:lv:hyb:HALF:HYBID:dvdd:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:dvdd:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:avdd:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:v125:i_rd")
}

record(ai, SVT:lv:hyb:HALF:HYBID:avdd:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:avdd:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:v125:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:dvdd:vn")
}

record(ai, SVT:lv:hyb:HALF:HYBID:v125:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:v125:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:hyb:HALF:HYBID:dvdd:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:avdd:vn")
}

record(ai, SVT:lv:hyb:HALF:HYBID:dvdd:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:dvdd:vn_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:avdd:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:v125:vn")
}

record(ai, SVT:lv:hyb:HALF:HYBID:avdd:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:avdd:vn_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:v125:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:dvdd:vf")
}

record(ai, SVT:lv:hyb:HALF:HYBID:v125:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:v125:vn_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:hyb:HALF:HYBID:dvdd:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:avdd:vf")
}

record(ai, SVT:lv:hyb:HALF:HYBID:dvdd:vf)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:dvdd:vf_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:avdd:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:v125:vf")
}

record(ai, SVT:lv:hyb:HALF:HYBID:avdd:vf)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:avdd:vf_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:v125:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:dvdd:v_set_rd")
}

record(ai, SVT:lv:hyb:HALF:HYBID:v125:vf) {
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:v125:vf_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:hyb:HALF:HYBID:dvdd:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:avdd:v_set_rd")
}

record(ai, SVT:lv:hyb:HALF:HYBID:dvdd:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:dvdd:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:avdd:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:v125:v_set_rd")
}

record(ai, SVT:lv:hyb:HALF:HYBID:avdd:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:avdd:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:v125:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")    
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:lv:hyb:HALF:HYBID:v125:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:hyb:HALF:HYBID:v125:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:hyb:HALF:HYBID:dvdd:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:hyb:HALF:HYBID:dvdd:v_set) {
  field(OUT, "SVT:lv:hyb:HALF:HYBID:dvdd:v_set_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:avdd:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:hyb:HALF:HYBID:avdd:v_set) {
  field(OUT, "SVT:lv:hyb:HALF:HYBID:avdd:v_set_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:hyb:HALF:HYBID:v125:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:hyb:HALF:HYBID:v125:v_set) {
  field(OUT, "SVT:lv:hyb:HALF:HYBID:v125:v_set_sub PP")
  field(DTYP,"Soft Channel")
}




"""
    s_flnk = "SVT:lv:hyb:HALF:NEXTHYB:dvdd:i_rd"
    records = []
    for half in ["bot","top"]:
        for hyb in range(0,18):
            rec = s
            if hyb==17:
                if half is "bot":
                    ss_flnk = s_flnk.replace("HALF","top")
                    rec = rec.replace("FLNKNEXTHYB",ss_flnk)
                    rec = rec.replace("NEXTHYB",str(0))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYB",str(hyb+1))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("HALF",half)
            records.append(rec)
    
    return records








def buildHybLVSwitch():


    s = """



record(sub,SVT:lv:hyb:HALF:HYBID:all:switch_sub)
{
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(SCAN,"Passive")
    field(FLNK,"SVT:lv:hyb:HALF:HYBID:all:switch_fanout")
}

record(bo, SVT:lv:hyb:HALF:HYBID:all:switch)
{
    field(OUT, "SVT:lv:hyb:HALF:HYBID:all:switch_sub PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "supervisory")
}

record(fanout,SVT:lv:hyb:HALF:HYBID:all:switch_fanout)
{
    field(LNK1,"SVT:lv:hyb:HALF:HYBID:dvdd:switch")
    field(LNK2,"SVT:lv:hyb:HALF:HYBID:avdd:switch")
    field(LNK3,"SVT:lv:hyb:HALF:HYBID:v125:switch")
}


record(bo, SVT:lv:hyb:HALF:HYBID:dvdd:switch)
{
    field(DOL, "SVT:lv:hyb:HALF:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}

record(bo, SVT:lv:hyb:HALF:HYBID:avdd:switch)
{
    field(DOL, "SVT:lv:hyb:HALF:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}

record(bo, SVT:lv:hyb:HALF:HYBID:v125:switch)
{
    field(DOL, "SVT:lv:hyb:HALF:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}



"""
    
    records = []
    for half in ["bot","top"]:
        for hyb in range(0,18):
            rec = s.replace("HYBID",str(hyb))
            rec = rec.replace("HALF",half)
            records.append(rec)
    
    return records




def buildHybTemp():

    

    s = """
record(sub,SVT:temp:hyb:HALF:HYBID:temp0:t_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:temp:hyb:HALF:HYBID:temp1:t_rd")
}

record(ai, SVT:temp:hyb:HALF:HYBID:temp0:t_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:temp:hyb:HALF:HYBID:temp0:t_rd_sub PP")
  field(DTYP,"Soft Channel")
}


record(sub,SVT:temp:hyb:HALF:HYBID:temp1:t_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:temp:hyb:HALF:HYBID:temp1:t_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:temp:hyb:HALF:HYBID:temp1:t_rd_sub PP")
  field(DTYP,"Soft Channel")
}
"""

    s_flnk = "SVT:temp:hyb:HALF:NEXTHYB:temp0:t_rd"
    records = []
    for half in ["bot","top"]:
        for hyb in range(0,18):
            rec = s
            if hyb==17:
                if half is "bot":
                    ss_flnk = s_flnk.replace("HALF","top")
                    rec = rec.replace("FLNKNEXTHYB",ss_flnk)
                    rec = rec.replace("NEXTHYB",str(0))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYB",str(hyb+1))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("HALF",half)
            records.append(rec)
    
    return records


def buildFebTemp():

    s = """
record(sub,SVT:temp:fe:FEBID:axixadc:t_rd_sub)
{
    field(SCAN, "Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:temp:fe:FEBID:FebTemp0:t_rd")
}

record(ai, SVT:temp:fe:FEBID:axixadc:t_rd) {
    field(SCAN, "Passive") field(PREC, "1")
    field(INP, "SVT:temp:fe:FEBID:axixadc:t_rd_sub PP")
    field(DTYP,"Soft Channel")
}

record(sub,SVT:temp:fe:FEBID:FebTemp0:t_rd_sub)
{
    field(SCAN, "Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:temp:fe:FEBID:FebTemp1:t_rd")
}

record(ai, SVT:temp:fe:FEBID:FebTemp0:t_rd) {
    field(SCAN, "Passive") field(PREC, "1")
    field(INP, "SVT:temp:fe:FEBID:FebTemp0:t_rd_sub PP")
    field(DTYP,"Soft Channel")
}

record(sub,SVT:temp:fe:FEBID:FebTemp1:t_rd_sub)
{
    field(SCAN, "Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"FLNKNEXTFEB")
}

record(ai, SVT:temp:fe:FEBID:FebTemp1:t_rd) {
    field(SCAN, "Passive") field(PREC, "1")
    field(INP, "SVT:temp:fe:FEBID:FebTemp1:t_rd_sub PP")
    field(DTYP,"Soft Channel")
}

"""

    s_flnk = "SVT:temp:fe:NEXTFEBID:axixadc:t_rd"

    records = []
    for feb in range(0,10):
        rec = s
        # take care of the flnk to the next feb
        if feb==9:
            rec = rec.replace("FLNKNEXTFEB","")
        else:
            rec = rec.replace("FLNKNEXTFEB",s_flnk)            
        rec = rec.replace("NEXTFEBID",str(feb+1))
        rec = rec.replace("FEBID",str(feb))
        records.append(rec)
    
    return records
