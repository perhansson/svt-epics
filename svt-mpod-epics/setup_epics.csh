setenv EPICS_HOST_ARCH linux-x86
setenv EPICS_BASE /u1/epics/base-3.14.12.2
setenv MEDM_BASE /u1/epics/extensions
setenv SNMP /u1/packages/net-snmp/builds/net-snmp-5.7.2.1
setenv QTDIR   /u1/packages/qt_4.7.4_x64
setenv QWTDIR  /u1/packages/qwt_6.0_x64
setenv PATH ${EPICS_BASE}/bin/${EPICS_HOST_ARCH}:${MEDM_BASE}/bin/${EPICS_HOST_ARCH}:${SNMP}/bin:${QTDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH ${EPICS_BASE}/lib/${EPICS_HOST_ARCH}:${SNMP}/lib:${QTDIR}/lib:${QWTDIR}/lib:${LD_LIBRARY_PATH}
