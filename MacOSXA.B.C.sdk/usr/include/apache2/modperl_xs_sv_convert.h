
/*
 * *********** WARNING **************
 * This file generated by ModPerl::WrapXS/0.01
 * Any changes made here will be lost
 * ***********************************
 * 01: /private/var/tmp/apache_mod_perl/apache_mod_perl-108~358/mod_perl-2.0.7/blib/lib/ModPerl/WrapXS.pm:706
 * 02: /private/var/tmp/apache_mod_perl/apache_mod_perl-108~358/mod_perl-2.0.7/blib/lib/ModPerl/WrapXS.pm:1167
 * 03: Makefile.PL:427
 * 04: Makefile.PL:329
 * 05: Makefile.PL:58
 */


#ifndef MODPERL_XS_SV_CONVERT_H
#define MODPERL_XS_SV_CONVERT_H

#define mp_xs_sv2_APR__Pool(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Pool derived object)"),0) ? \
INT2PTR(apr_pool_t *, SvIV((SV*)SvRV(sv))) : (apr_pool_t *)NULL)

#define mp_xs_APR__Pool_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Pool", (void*)ptr)

#define mp_xs_sv2_APR__UUID(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::UUID derived object)"),0) ? \
INT2PTR(apr_uuid_t *, SvIV((SV*)SvRV(sv))) : (apr_uuid_t *)NULL)

#define mp_xs_sv2_uuid mp_xs_sv2_APR__UUID

#define mp_xs_APR__UUID_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::UUID", (void*)ptr)

#define mp_xs_uuid_2obj mp_xs_APR__UUID_2obj

#define mp_xs_sv2_Apache2__Module(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Module derived object)"),0) ? \
INT2PTR(module_struct *, SvIV((SV*)SvRV(sv))) : (module_struct *)NULL)

#define mp_xs_Apache2__Module_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Module", (void*)ptr)

#define mp_xs_sv2_unsigned(sv) (unsigned)SvUV(sv)

#define mp_xs_sv2_APR__Bucket(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Bucket derived object)"),0) ? \
INT2PTR(apr_bucket *, SvIV((SV*)SvRV(sv))) : (apr_bucket *)NULL)

#define mp_xs_APR__Bucket_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Bucket", (void*)ptr)

#define mp_xs_sv2_Apache2__Process(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Process derived object)"),0) ? \
INT2PTR(process_rec *, SvIV((SV*)SvRV(sv))) : (process_rec *)NULL)

#define mp_xs_Apache2__Process_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Process", (void*)ptr)

#define mp_xs_sv2_apr_time_t(sv) (apr_time_t)SvNV(sv)

#define mp_xs_sv2_apr_uint64_t(sv) (apr_uint64_t)SvNV(sv)

#define mp_xs_sv2_apr_port_t(sv) (apr_port_t)SvIV(sv)

#define mp_xs_sv2_Apache2__ConfVector(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::ConfVector derived object)"),0) ? \
INT2PTR(ap_conf_vector_t *, SvIV((SV*)SvRV(sv))) : (ap_conf_vector_t *)NULL)

#define mp_xs_Apache2__ConfVector_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::ConfVector", (void*)ptr)

#define mp_xs_sv2_Apache2__Filter(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Filter derived object)"),0) ? \
INT2PTR(ap_filter_t *, SvIV((SV*)SvRV(sv))) : (ap_filter_t *)NULL)

#define mp_xs_Apache2__Filter_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Filter", (void*)ptr)

#define mp_xs_sv2_apr_uid_t(sv) (apr_uid_t)SvIV(sv)

#define mp_xs_sv2_gid_t(sv) (gid_t)SvIV(sv)

#define mp_xs_sv2_apr_read_type_e(sv) (apr_read_type_e)SvIV(sv)

#define mp_xs_sv2_apr_off_t(sv) (apr_off_t)SvIV(sv)

#define mp_xs_sv2_Apache2__FilterRec(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::FilterRec derived object)"),0) ? \
INT2PTR(ap_filter_rec_t *, SvIV((SV*)SvRV(sv))) : (ap_filter_rec_t *)NULL)

#define mp_xs_Apache2__FilterRec_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::FilterRec", (void*)ptr)

#define mp_xs_sv2_Apache2__SubRequest(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::SubRequest derived object)"),0) ? \
INT2PTR(subrequest_rec *, SvIV((SV*)SvRV(sv))) : (subrequest_rec *)NULL)

#define mp_xs_Apache2__SubRequest_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::SubRequest", (void*)ptr)

#define mp_xs_sv2_int(sv) (int)SvIV(sv)

#define mp_xs_sv2_APR__MD5(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::MD5 derived object)"),0) ? \
INT2PTR(apr_md5_ctx_t *, SvIV((SV*)SvRV(sv))) : (apr_md5_ctx_t *)NULL)

#define mp_xs_APR__MD5_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::MD5", (void*)ptr)

#define mp_xs_sv2_apr_int64_t(sv) (apr_int64_t)SvNV(sv)

#define mp_xs_sv2_Apache2__PipedLog(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::PipedLog derived object)"),0) ? \
INT2PTR(piped_log *, SvIV((SV*)SvRV(sv))) : (piped_log *)NULL)

#define mp_xs_Apache2__PipedLog_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::PipedLog", (void*)ptr)

#define mp_xs_sv2_APR__TableEntry(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::TableEntry derived object)"),0) ? \
INT2PTR(apr_table_entry_t *, SvIV((SV*)SvRV(sv))) : (apr_table_entry_t *)NULL)

#define mp_xs_APR__TableEntry_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::TableEntry", (void*)ptr)

#define mp_xs_sv2_Apache2__ServerAddr(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::ServerAddr derived object)"),0) ? \
INT2PTR(server_addr_rec *, SvIV((SV*)SvRV(sv))) : (server_addr_rec *)NULL)

#define mp_xs_Apache2__ServerAddr_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::ServerAddr", (void*)ptr)

#define mp_xs_sv2_APR__Table(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Table derived object)"),0) ? \
INT2PTR(apr_table_t *, SvIV((SV*)SvRV(sv))) : (apr_table_t *)NULL)

#define mp_xs_sv2_table mp_xs_sv2_APR__Table

#define mp_xs_APR__Table_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Table", (void*)ptr)

#define mp_xs_table_2obj mp_xs_APR__Table_2obj

#define mp_xs_sv2_U32(sv) (U32)SvUV(sv)

#define mp_xs_sv2_APR__URI(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::URI derived object)"),0) ? \
INT2PTR(apr_uri_t *, SvIV((SV*)SvRV(sv))) : (apr_uri_t *)NULL)

#define mp_xs_APR__URI_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::URI", (void*)ptr)

#define mp_xs_sv2_Apache2__ConfigFile(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::ConfigFile derived object)"),0) ? \
INT2PTR(ap_configfile_t *, SvIV((SV*)SvRV(sv))) : (ap_configfile_t *)NULL)

#define mp_xs_Apache2__ConfigFile_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::ConfigFile", (void*)ptr)

#define mp_xs_sv2_Apache2__Command(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Command derived object)"),0) ? \
INT2PTR(command_rec *, SvIV((SV*)SvRV(sv))) : (command_rec *)NULL)

#define mp_xs_Apache2__Command_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Command", (void*)ptr)

#define mp_xs_sv2_APR__SockInfo(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::SockInfo derived object)"),0) ? \
INT2PTR(apr_os_sock_info_t *, SvIV((SV*)SvRV(sv))) : (apr_os_sock_info_t *)NULL)

#define mp_xs_APR__SockInfo_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::SockInfo", (void*)ptr)

#define mp_xs_sv2_APR__ArrayHeader(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::ArrayHeader derived object)"),0) ? \
INT2PTR(apr_array_header_t *, SvIV((SV*)SvRV(sv))) : (apr_array_header_t *)NULL)

#define mp_xs_APR__ArrayHeader_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::ArrayHeader", (void*)ptr)

#define mp_xs_sv2_apr_status_t(sv) (apr_status_t)SvIV(sv)

#define mp_xs_sv2_status mp_xs_sv2_apr_status_t

#define mp_xs_sv2_apr_ino_t(sv) (apr_ino_t)SvIV(sv)

#define mp_xs_sv2_Apache2__MethodList(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::MethodList derived object)"),0) ? \
INT2PTR(ap_method_list_t *, SvIV((SV*)SvRV(sv))) : (ap_method_list_t *)NULL)

#define mp_xs_Apache2__MethodList_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::MethodList", (void*)ptr)

#define mp_xs_sv2_APR__Mmap(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Mmap derived object)"),0) ? \
INT2PTR(apr_mmap_t *, SvIV((SV*)SvRV(sv))) : (apr_mmap_t *)NULL)

#define mp_xs_APR__Mmap_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Mmap", (void*)ptr)

#define mp_xs_sv2_APR__ThreadMutex(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::ThreadMutex derived object)"),0) ? \
INT2PTR(apr_thread_mutex_t *, SvIV((SV*)SvRV(sv))) : (apr_thread_mutex_t *)NULL)

#define mp_xs_APR__ThreadMutex_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::ThreadMutex", (void*)ptr)

#define mp_xs_sv2_apr_uint16_t(sv) (apr_uint16_t)SvIV(sv)

#define mp_xs_sv2_APR__Socket(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Socket derived object)"),0) ? \
INT2PTR(apr_socket_t *, SvIV((SV*)SvRV(sv))) : (apr_socket_t *)NULL)

#define mp_xs_APR__Socket_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Socket", (void*)ptr)

#define mp_xs_sv2_apr_int32_t(sv) (apr_int32_t)SvIV(sv)

#define mp_xs_sv2_size_t(sv) (size_t)SvIV(sv)

#define mp_xs_sv2_APR__BucketAlloc(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::BucketAlloc derived object)"),0) ? \
INT2PTR(apr_bucket_alloc_t *, SvIV((SV*)SvRV(sv))) : (apr_bucket_alloc_t *)NULL)

#define mp_xs_APR__BucketAlloc_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::BucketAlloc", (void*)ptr)

#define mp_xs_sv2_apr_interval_time_t(sv) (apr_interval_time_t)SvNV(sv)

#define mp_xs_sv2_APR__ThreadRWLock(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::ThreadRWLock derived object)"),0) ? \
INT2PTR(apr_thread_rwlock_t *, SvIV((SV*)SvRV(sv))) : (apr_thread_rwlock_t *)NULL)

#define mp_xs_APR__ThreadRWLock_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::ThreadRWLock", (void*)ptr)

#define mp_xs_sv2_apr_ssize_t(sv) (apr_ssize_t)SvIV(sv)

#define mp_xs_sv2_APR__GetoptOption(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::GetoptOption derived object)"),0) ? \
INT2PTR(apr_getopt_option_t *, SvIV((SV*)SvRV(sv))) : (apr_getopt_option_t *)NULL)

#define mp_xs_APR__GetoptOption_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::GetoptOption", (void*)ptr)

#define mp_xs_sv2_long(sv) (long)SvIV(sv)

#define mp_xs_sv2_ap_conn_keepalive_e(sv) (ap_conn_keepalive_e)SvIV(sv)

#define mp_xs_sv2_ap_scoreboard_e(sv) (ap_scoreboard_e)SvIV(sv)

#define mp_xs_sv2_ap_mgmt_type_e(sv) (ap_mgmt_type_e)SvIV(sv)

#define mp_xs_sv2_APR__Process(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Process derived object)"),0) ? \
INT2PTR(apr_proc_t *, SvIV((SV*)SvRV(sv))) : (apr_proc_t *)NULL)

#define mp_xs_APR__Process_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Process", (void*)ptr)

#define mp_xs_sv2_Apache2__ServerRec(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::ServerRec derived object)"),0) ? \
INT2PTR(server_rec *, SvIV((SV*)SvRV(sv))) : (server_rec *)NULL)

#define mp_xs_Apache2__ServerRec_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::ServerRec", (void*)ptr)

#define mp_xs_sv2_Apache2__FilterType(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::FilterType derived object)"),0) ? \
INT2PTR(ap_filter_type *, SvIV((SV*)SvRV(sv))) : (ap_filter_type *)NULL)

#define mp_xs_Apache2__FilterType_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::FilterType", (void*)ptr)

#define mp_xs_sv2_Apache2__OutputFilter(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::OutputFilter derived object)"),0) ? \
INT2PTR(modperl_filter_t *, SvIV((SV*)SvRV(sv))) : (modperl_filter_t *)NULL)

#define mp_xs_Apache2__OutputFilter_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::OutputFilter", (void*)ptr)

#define mp_xs_sv2_Apache2__Directive(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Directive derived object)"),0) ? \
INT2PTR(ap_directive_t *, SvIV((SV*)SvRV(sv))) : (ap_directive_t *)NULL)

#define mp_xs_Apache2__Directive_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Directive", (void*)ptr)

#define mp_xs_sv2_Apache2__RequestRec(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::RequestRec derived object)"),0) ? \
INT2PTR(request_rec *, SvIV((SV*)SvRV(sv))) : (request_rec *)NULL)

#define mp_xs_sv2_r mp_xs_sv2_Apache2__RequestRec

#define mp_xs_Apache2__RequestRec_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::RequestRec", (void*)ptr)

#define mp_xs_r_2obj mp_xs_Apache2__RequestRec_2obj

#define mp_xs_sv2_apr_gid_t(sv) (apr_gid_t)SvIV(sv)

#define mp_xs_sv2_ap_input_mode_t(sv) (ap_input_mode_t)SvIV(sv)

#define mp_xs_sv2_APR__Finfo(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Finfo derived object)"),0) ? \
INT2PTR(apr_finfo_t *, SvIV((SV*)SvRV(sv))) : (apr_finfo_t *)NULL)

#define mp_xs_APR__Finfo_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Finfo", (void*)ptr)

#define mp_xs_sv2_APR__SHA1(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::SHA1 derived object)"),0) ? \
INT2PTR(apr_sha1_ctx_t *, SvIV((SV*)SvRV(sv))) : (apr_sha1_ctx_t *)NULL)

#define mp_xs_APR__SHA1_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::SHA1", (void*)ptr)

#define mp_xs_sv2_apr_filetype_e(sv) (apr_filetype_e)SvIV(sv)

#define mp_xs_sv2_apr_fileperms_t(sv) (apr_fileperms_t)SvIV(sv)

#define mp_xs_sv2_APR__IpSubnet(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::IpSubnet derived object)"),0) ? \
INT2PTR(apr_ipsubnet_t *, SvIV((SV*)SvRV(sv))) : (apr_ipsubnet_t *)NULL)

#define mp_xs_APR__IpSubnet_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::IpSubnet", (void*)ptr)

#define mp_xs_sv2_I32(sv) (I32)SvIV(sv)

#define mp_xs_sv2_apr_size_t(sv) (apr_size_t)SvUV(sv)

#define mp_xs_sv2_Apache2__Connection(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::Connection derived object)"),0) ? \
INT2PTR(conn_rec *, SvIV((SV*)SvRV(sv))) : (conn_rec *)NULL)

#define mp_xs_sv2_connection mp_xs_sv2_Apache2__Connection

#define mp_xs_Apache2__Connection_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::Connection", (void*)ptr)

#define mp_xs_connection_2obj mp_xs_Apache2__Connection_2obj

#define mp_xs_sv2_time_t(sv) (time_t)SvNV(sv)

#define mp_xs_sv2_apr_lockmech_e(sv) (apr_lockmech_e)SvIV(sv)

#define mp_xs_sv2_APR__Getopt(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Getopt derived object)"),0) ? \
INT2PTR(apr_getopt_t *, SvIV((SV*)SvRV(sv))) : (apr_getopt_t *)NULL)

#define mp_xs_APR__Getopt_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Getopt", (void*)ptr)

#define mp_xs_sv2_apr_dev_t(sv) (apr_dev_t)SvNV(sv)

#define mp_xs_sv2_uid_t(sv) (uid_t)SvIV(sv)

#define mp_xs_sv2_APR__BucketType(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::BucketType derived object)"),0) ? \
INT2PTR(apr_bucket_type_t *, SvIV((SV*)SvRV(sv))) : (apr_bucket_type_t *)NULL)

#define mp_xs_APR__BucketType_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::BucketType", (void*)ptr)

#define mp_xs_sv2_apr_int16_t(sv) (apr_int16_t)SvIV(sv)

#define mp_xs_sv2_APR__ExplodedTime(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::ExplodedTime derived object)"),0) ? \
INT2PTR(apr_time_exp_t *, SvIV((SV*)SvRV(sv))) : (apr_time_exp_t *)NULL)

#define mp_xs_APR__ExplodedTime_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::ExplodedTime", (void*)ptr)

#define mp_xs_sv2_double(sv) (double)SvNV(sv)

#define mp_xs_sv2_apr_socklen_t(sv) (apr_socklen_t)SvIV(sv)

#define mp_xs_sv2_APR__InAddr(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::InAddr derived object)"),0) ? \
INT2PTR(apr_in_addr_t *, SvIV((SV*)SvRV(sv))) : (apr_in_addr_t *)NULL)

#define mp_xs_APR__InAddr_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::InAddr", (void*)ptr)

#define mp_xs_sv2_apr_locktype_e(sv) (apr_locktype_e)SvIV(sv)

#define mp_xs_sv2_APR__Brigade(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Brigade derived object)"),0) ? \
INT2PTR(apr_bucket_brigade *, SvIV((SV*)SvRV(sv))) : (apr_bucket_brigade *)NULL)

#define mp_xs_APR__Brigade_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Brigade", (void*)ptr)

#define mp_xs_sv2_APR__SockAddr(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::SockAddr derived object)"),0) ? \
INT2PTR(apr_sockaddr_t *, SvIV((SV*)SvRV(sv))) : (apr_sockaddr_t *)NULL)

#define mp_xs_APR__SockAddr_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::SockAddr", (void*)ptr)

#define mp_xs_sv2_APR__Hash(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::Hash derived object)"),0) ? \
INT2PTR(apr_hash_t *, SvIV((SV*)SvRV(sv))) : (apr_hash_t *)NULL)

#define mp_xs_APR__Hash_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::Hash", (void*)ptr)

#define mp_xs_sv2_Apache2__CmdParms(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an Apache2::CmdParms derived object)"),0) ? \
INT2PTR(cmd_parms *, SvIV((SV*)SvRV(sv))) : (cmd_parms *)NULL)

#define mp_xs_Apache2__CmdParms_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "Apache2::CmdParms", (void*)ptr)

#define mp_xs_sv2_APR__HashIndex(sv) \
((SvROK(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG)) \
|| (Perl_croak(aTHX_ "argument is not a blessed reference (expecting an APR::HashIndex derived object)"),0) ? \
INT2PTR(apr_hash_index_t *, SvIV((SV*)SvRV(sv))) : (apr_hash_index_t *)NULL)

#define mp_xs_APR__HashIndex_2obj(ptr) \
sv_setref_pv(sv_newmortal(), "APR::HashIndex", (void*)ptr)

#define mp_xs_sv2_pid_t(sv) (pid_t)SvIV(sv)

#define mp_xs_sv2_apr_uint32_t(sv) (apr_uint32_t)SvIV(sv)


#endif /* MODPERL_XS_SV_CONVERT_H */
