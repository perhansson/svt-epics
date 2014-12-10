setenv EPICS_HOST_ARCH linux-x86_64
setenv EPICS_BASE /u1/packages/base-3.14.12.2
setenv MEDM_BASE /u1/packages/extensions
setenv SNMP /u1/packages/net-snmp/builds/net-snmp-5.7.2.1
setenv QTDIR   /u1/packages/qt_4.7.4_x64
setenv QWTDIR  /u1/packages/qwt_6.0_x64
setenv PATH ${EPICS_BASE}/bin/linux-x86:${MEDM_BASE}/bin/linux-x86:${SNMP}/bin:${QTDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH ${EPICS_BASE}/lib/linux-x86_64:${SNMP}/lib:${QTDIR}/lib:${QWTDIR}/lib:${LD_LIBRARY_PATH}