

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
  field(FLNK,"SVT:temp:hyb:HALF:HYBID:temp0:t_rd_sub")
  field(INP, "SVT:temp:hyb:HALF:HYBID:temp0:t_rd_sub CPP")
  field(DTYP,"Soft Channel")
}


record(sub,SVT:temp:hyb:HALF:HYBID:temp1:t_rd_sub)
{
    field(SCAN,"Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:temp:hyb:HALF:NEXTHYBID:temp0:t_rd")
}

record(ai, SVT:temp:hyb:HALF:HYBID:temp1:t_rd) {
  field(SCAN, "Passive") field(PREC, "1")
  field(FLNK,"SVT:temp:hyb:HALF:HYBID:temp1:t_rd_sub")
  field(INP, "SVT:temp:hyb:HALF:HYBID:temp1:t_rd_sub CPP")
  field(DTYP,"Soft Channel")
}
"""
    records = []
    for half in ["bot","top"]:
        for hyb in range(1,18):
            rec = s.replace("HYBID",str(hyb))
            if hyb==17:
                if half=="bot":
                    rec = rec.replace("NEXTHYB",str(1))
            else:
                rec = rec.replace("NEXTHYB",str(hyb+1))
            records.append(rec)
    return records


def buildFebTempAxi():

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
    field(FLNK,"SVT:temp:fe:FEBID:axixadc:t_rd_sub")
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
    field(FLNK,"SVT:temp:fe:FEBID:FebTemp0:t_rd_sub")
    field(INP, "SVT:temp:fe:FEBID:FebTemp0:t_rd_sub PP")
    field(DTYP,"Soft Channel")
}

record(sub,SVT:temp:fe:FEBID:FebTemp1:t_rd_sub)
{
    field(SCAN, "Passive")
    field(INAM,"subTempInit")
    field(SNAM,"subTempProcess")
    field(FLNK,"SVT:lv:hyb:bot:FEBID:dvdd:i_rd")
}

record(ai, SVT:temp:fe:FEBID:FebTemp1:t_rd) {
    field(SCAN, "Passive") field(PREC, "1")
    field(FLNK,"SVT:temp:fe:FEBID:FebTemp1:t_rd_sub")
    field(INP, "SVT:temp:fe:FEBID:FebTemp1:t_rd_sub PP")
    field(DTYP,"Soft Channel")
}

"""

    records = []
    for feb in range(0,10):
        rec = s.replace("FEBID",str(feb))
        records.append(rec)
    
    return records
