import sys

class FEB:
    def __init__(self,febid,febchs,half,hybridlist):
        self.id = febid
        self.febchs = febchs
        self.half = half
        self.hybrids = hybridlist

            
# mapping between hybrid and FEB IDs
febs= [
    FEB(0,[0,1],"top",[0,1]),
    FEB(1,[0,1,2,3],"top",[2,3,4,5]),
    FEB(2,[0,1,2,3],"top",[6,7,8,9]),
    FEB(3,[0,1,2,3],"top",[10,11,12,13]),
    FEB(4,[0,1,2,3],"top",[14,15,16,17]),
    FEB(5,[0,1,2,3],"bot",[2,3,4,5]),
    FEB(6,[0,1],"bot",[0,1]),
    FEB(7,[0,1,2,3],"bot",[6,7,8,9]),
    FEB(8,[0,1,2,3],"bot",[10,11,12,13]),
    FEB(9,[0,1,2,3],"bot",[14,15,16,17])
    ]





def getFebId(half,hyb):
    febid = -1
    for feb in febs:
        if feb.half is half:
            if hyb in feb.hybrids:
                febid = feb.id
    return febid

def getFebCh(half,hyb):
    febch = -1
    for feb in febs:
        if feb.half is half:
            for i in range(len(feb.hybrids)):
                if feb.hybrids[i]==hyb:
                    febch = feb.febchs[i]

    return febch



def buildSvtDaqMapHyb():
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


def buildSvtDaqMapFebch():
    s = """

record(longin, SVT:daq:map:HALF:HYBID:febch) {
    field(VAL,"FEBCH")
}

    """
    records = []
    for half in ["bot","top"]:
        for hyb in range(0,18):
            feb = getFebCh(half,hyb)
            if feb<0:
                print "Invalid feb ch for hyb ", hyb, " and half ", half
                sys.exit(1)
            rec = s
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("HALF",half)            
            rec = rec.replace("FEBCH",str(feb))
            records.append(rec)
    
    return records


def buildSvtDaqMap():
    recs = buildSvtDaqMapHyb()
    recs2 = buildSvtDaqMapFebch()
    for r in recs2:
        recs.append(r)
    return recs



def buildHybLV():


    s = """


record(sub,SVT:lv:FEBID:HYBID:dvdd:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:avdd:i_rd")
}

record(ai, SVT:lv:FEBID:HYBID:dvdd:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:dvdd:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:avdd:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:v125:i_rd")
}

record(ai, SVT:lv:FEBID:HYBID:avdd:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:avdd:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:i_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:dvdd:vn")
}

record(ai, SVT:lv:FEBID:HYBID:v125:i_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:v125:i_rd_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:FEBID:HYBID:dvdd:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:avdd:vn")
}

record(ai, SVT:lv:FEBID:HYBID:dvdd:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:dvdd:vn_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:avdd:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:v125:vn")
}

record(ai, SVT:lv:FEBID:HYBID:avdd:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:avdd:vn_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:vn_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:dvdd:vf")
}

record(ai, SVT:lv:FEBID:HYBID:v125:vn)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:v125:vn_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:FEBID:HYBID:dvdd:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:avdd:vf")
}

record(ai, SVT:lv:FEBID:HYBID:dvdd:vf)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:dvdd:vf_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:avdd:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:v125:vf")
}

record(ai, SVT:lv:FEBID:HYBID:avdd:vf)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:avdd:vf_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:vf_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:dvdd:v_set_rd")
}

record(ai, SVT:lv:FEBID:HYBID:v125:vf) {
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:v125:vf_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:FEBID:HYBID:dvdd:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:avdd:v_set_rd")
}

record(ai, SVT:lv:FEBID:HYBID:dvdd:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:dvdd:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:avdd:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"SVT:lv:FEBID:HYBID:v125:v_set_rd")
}

record(ai, SVT:lv:FEBID:HYBID:avdd:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:avdd:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:v_set_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")    
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:lv:FEBID:HYBID:v125:v_set_rd)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:v125:v_set_rd_sub PP")
  field(DTYP,"Soft Channel")
}






record(sub,SVT:lv:FEBID:HYBID:dvdd:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:FEBID:HYBID:dvdd:v_set) {
  field(OUT, "SVT:lv:FEBID:HYBID:dvdd:v_set_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:avdd:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:FEBID:HYBID:avdd:v_set) {
  field(OUT, "SVT:lv:FEBID:HYBID:avdd:v_set_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:v_set_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ao, SVT:lv:FEBID:HYBID:v125:v_set) {
  field(OUT, "SVT:lv:FEBID:HYBID:v125:v_set_sub PP")
  field(DTYP,"Soft Channel")
}




"""
    s_flnk = "SVT:lv:NEXTFEBID:NEXTHYBID:dvdd:i_rd"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))					
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTFEBID",str(feb))
                rec = rec.replace("NEXTHYBID",str(hyb+1))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records








def buildHybLVStat():


    s = """


record(sub,SVT:lv:FEBID:HYBID:dvdd:stat_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ai, SVT:lv:FEBID:HYBID:dvdd:stat)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:dvdd:stat_sub PP")
  field(DTYP,"Soft Channel")
}


record(sub,SVT:lv:FEBID:HYBID:avdd:stat_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ai, SVT:lv:FEBID:HYBID:avdd:stat)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:avdd:stat_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:v125:stat_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
}

record(ai, SVT:lv:FEBID:HYBID:v125:stat)
{
  field(SCAN, "Passive") field(PREC, "3")
  field(INP, "SVT:lv:FEBID:HYBID:v125:stat_sub PP")
  field(DTYP,"Soft Channel")
}

record(calc, SVT:lv:FEBID:HYBID:stat)
{
  field(SCAN, "Passive")
  field(CALC, "A&&B&&C")
  field(INPA, "SVT:lv:FEBID:HYBID:dvdd:stat.VAL PP")
  field(INPB, "SVT:lv:FEBID:HYBID:avdd:stat.VAL PP")
  field(INPC, "SVT:lv:FEBID:HYBID:v125:stat.VAL PP")
  field(FLNK,"FLNKNEXTHYB")
  
}



"""


    s_flnk = "SVT:lv:NEXTFEBID:NEXTHYBID:stat"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))					
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTFEBID",str(feb))
                rec = rec.replace("NEXTHYBID",str(hyb+1))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records


  







def buildHybLVSwitch():


    s = """



record(sub,SVT:lv:FEBID:HYBID:all:switch_sub)
{
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(SCAN,"Passive")
    field(FLNK,"SVT:lv:FEBID:HYBID:all:switch_fanout")
}

record(bo, SVT:lv:FEBID:HYBID:all:switch)
{
    field(OUT, "SVT:lv:FEBID:HYBID:all:switch_sub PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "supervisory")
}

record(fanout,SVT:lv:FEBID:HYBID:all:switch_fanout)
{
    field(LNK1,"SVT:lv:FEBID:HYBID:dvdd:switch")
    field(LNK2,"SVT:lv:FEBID:HYBID:avdd:switch")
    field(LNK3,"SVT:lv:FEBID:HYBID:v125:switch")
}


record(bo, SVT:lv:FEBID:HYBID:dvdd:switch)
{
    field(DOL, "SVT:lv:FEBID:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}

record(bo, SVT:lv:FEBID:HYBID:avdd:switch)
{
    field(DOL, "SVT:lv:FEBID:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}

record(bo, SVT:lv:FEBID:HYBID:v125:switch)
{
    field(DOL, "SVT:lv:FEBID:HYBID:all:switch PP")
    field(DTYP,"Soft Channel")
    field(ZNAM, "Off")
    field(ONAM, "On")
    field(OMSL, "closed_loop")
}



"""
    
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    


    s = """
record(dfanout,SVT:lv:hyb:FEBID:all:switch_fanout)
{
    field(OUTA,"SVT:lv:hyb:FEBID:0:all:switch.VAL PP")
    field(OUTB,"SVT:lv:hyb:FEBID:1:all:switch.VAL PP")
    field(OUTC,"SVT:lv:hyb:FEBID:2:all:switch.VAL PP")
    field(OUTD,"SVT:lv:hyb:FEBID:3:all:switch.VAL PP")
}

record(bo, SVT:lv:hyb:FEBID:all:switch) 
{
  field(OUT, "SVT:lv:hyb:FEBID:all:switch_fanout PP")
  field(ZNAM, "Off")
  field(ONAM, "On")
}

"""
    
  
    
    for feb in range(0,10):
        rec = s.replace("FEBID",str(feb))
        records.append(rec)
    
    
    
    return records







def buildHybTemp():

    

    s = """
record(sub,SVT:temp:hyb:FEBID:HYBID:temp0:t_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:temp:hyb:FEBID:HYBID:temp1:t_rd")
}

record(ai, SVT:temp:hyb:FEBID:HYBID:temp0:t_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:temp:hyb:FEBID:HYBID:temp0:t_rd_sub PP")
  field(DTYP,"Soft Channel")
}


record(sub,SVT:temp:hyb:FEBID:HYBID:temp1:t_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:temp:hyb:FEBID:HYBID:temp1:t_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:temp:hyb:FEBID:HYBID:temp1:t_rd_sub PP")
  field(DTYP,"Soft Channel")
}
"""

    s_flnk = "SVT:temp:hyb:NEXTFEBID:NEXTHYBID:temp0:t_rd"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYBID",str(hyb+1))
                rec = rec.replace("NEXTFEBID",str(feb))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
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


def buildHybSync():

    

    s = """
record(sub,SVT:lv:hyb:FEBID:HYBID:sync:sync_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:lv:hyb:FEBID:HYBID:sync:sync_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:;v:hyb:FEBID:HYBID:sync:sync_rd_sub PP")
  field(DTYP,"Soft Channel")
}
"""

    s_flnk = "SVT:lv:hyb:NEXTFEBID:NEXTHYBID:sync:sync_rd"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYBID",str(hyb+1))
                rec = rec.replace("NEXTFEBID",str(feb))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records


def buildHybSync():

    

    s = """
record(sub,SVT:lv:FEBID:HYBID:sync:sync_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:lv:FEBID:HYBID:sync:sync_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:lv:FEBID:HYBID:sync:sync_rd_sub PP")
  field(DTYP,"Soft Channel")
}
"""

    s_flnk = "SVT:lv:NEXTFEBID:NEXTHYBID:sync:sync_rd"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYBID",str(hyb+1))
                rec = rec.replace("NEXTFEBID",str(feb))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records


def buildDpmMap():

    

    s = """
record(sub,SVT:lv:FEBID:HYBID:dpm:dpm_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
	field(FLNK,"SVT:lv:FEBID:HYBID:datapath:datapath_rd")
}

record(ai, SVT:lv:FEBID:HYBID:dpm:dpm_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:lv:FEBID:HYBID:dpm:dpm_rd_sub PP")
  field(DTYP,"Soft Channel")
}

record(sub,SVT:lv:FEBID:HYBID:datapath:datapath_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subLVInit")
    field(SNAM,"subLVProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:lv:FEBID:HYBID:datapath:datapath_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(INP, "SVT:lv:FEBID:HYBID:datapath:datapath_rd_sub PP")
  field(DTYP,"Soft Channel")
}

"""

    s_flnk = "SVT:lv:NEXTFEBID:NEXTHYBID:dpm:dpm_rd"
    records = []
    for feb in range(0,10):
        for hyb in range(0,4):
            rec = s
            if hyb==3:
                if feb < 9:
                    rec = rec.replace("FLNKNEXTHYB",s_flnk)
                    rec = rec.replace("NEXTHYBID",str(0))
                    rec = rec.replace("NEXTFEBID",str(feb+1))
                else:
                    rec = rec.replace("FLNKNEXTHYB","")                    
            else:
                rec = rec.replace("FLNKNEXTHYB",s_flnk)
                rec = rec.replace("NEXTHYBID",str(hyb+1))
                rec = rec.replace("NEXTFEBID",str(feb))
            rec = rec.replace("HYBID",str(hyb))
            rec = rec.replace("FEBID",str(feb))
            records.append(rec)
    
    return records



def buildFebPhysMap():
	
    s = """
record(sub,SVT:daq:map:LAYER:febid_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subPollDaqMapInit")
    field(SNAM,"subPollDaqMapProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(ai, SVT:daq:map:LAYER:febid) {
  field(SCAN, "Passive") field(PREC, "0")
  field(INP, "SVT:daq:map:LAYER:febid_sub PP")
  field(DTYP,"Soft Channel")
}


"""	
    s_flnk = "SVT:daq:map:NEXTLAYER:febid"
    records = []
    layers = ['l1t','l1b','l23t','l23b','l4t','l4b','l5t','l5b','l6t','l6b']
    for feb in range(0,len(layers)):
        rec = s
        if feb <9:
			rec = rec.replace("FLNKNEXTHYB",s_flnk)
			rec = rec.replace("NEXTLAYER",layers[feb+1])
        else:
			rec = rec.replace("FLNKNEXTHYB","")			
        rec = rec.replace("LAYER",layers[feb])
        records.append(rec)
	
    s = """
record(sub,SVT:daq:map:FEBID:layer_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subPollDaqMapInit")
    field(SNAM,"subPollDaqMapProcess")
    field(FLNK,"FLNKNEXTHYB")
}

record(longin, SVT:daq:map:FEBID:layer) {
  field(SCAN, "Passive") 
  field(INP, "SVT:daq:map:FEBID:layer_sub PP")
  field(DTYP,"Soft Channel")
}


"""	
    s_flnk = "SVT:daq:map:NEXTFEBID:layer"
    for feb in range(0,10):
        rec = s
        if feb <9:
			rec = rec.replace("FLNKNEXTHYB",s_flnk)
			rec = rec.replace("NEXTFEBID",str(feb+1))
        else:
			rec = rec.replace("FLNKNEXTHYB","")			
        rec = rec.replace("FEBID",str(feb))
        records.append(rec)
        
    return records

