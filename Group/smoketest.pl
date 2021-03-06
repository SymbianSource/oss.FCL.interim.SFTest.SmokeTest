#!/usr/bin/perl

# Copyright (c) 2009 Symbian Foundation Ltd
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Symbian Foundation Ltd - initial contribution.
#	Maciej Seroka, maciejs@symbian.org
#
# Description:
#   Script to build ATS test drop for Smoketest.

use strict;
use Getopt::Long;
use File::Copy;
use File::Path;

my $target;
my $ats_version;
my $help;

sub usage($);
sub help();
sub usage_error();

my %optmap = (  'target' => \$target,
				'ats-version' => \$ats_version,
				'help' => \$help);

GetOptions(\%optmap,
          'target=s',
		  'ats-version=s',
		  'help!') 
          or usage_error();

if ($help) {
	help();
}

# --target is not mandatory.
if ((!defined($target)) || (lc($target) eq "winscw")) {
    $target = "WINSCW";
	print "Building ATS test drop for Emulator... \n";
}
elsif (lc($target) eq "syborg") {
    $target = "SYBORG";
    print "Building ATS test drop for Syborg... \n";
}
else {
	# unknown target.
    usage_error();
}

# --ats-version is not mandatory.
if (!defined($ats_version)) { # Set ats3 by default
	$ats_version = "ats3" 
}

unlink "smoketest.zip";
rmtree "temp";
mkpath "temp";

my $epoc=$ENV{'EPOCROOT'} . "epoc32/";

if ($target eq "WINSCW") { # Build ATS test drop for Emulator.
	mkpath "temp/smoketest/general";
	mkpath "temp/smoketest/winscw_udeb";
	mkpath "temp/smoketest/sms/general";
	mkpath "temp/smoketest/emailmessage/general";
	mkpath "temp/smoketest/emailsettings/general";
	mkpath "temp/smoketest/wallpaper/general";
	mkpath "temp/smoketest/appinstall/general";
	mkpath "temp/smoketest/10003a3f/general";
	mkpath "temp/smoketest/resource/general";
	mkpath "temp/smoketest/localisation/general";
	mkpath "temp/smoketest/bitmap/general";
	mkpath "temp/smoketest/ethernet/general";
	mkpath "temp/smoketest/web/general";

	if (lc($ats_version) eq "ats3") { copy("smoketest.xml",				"temp/test.xml"); } # Use ATS3 test plan.
	if (lc($ats_version) eq "ats4") { copy("ats4_smoketest.xml",		"temp/test.xml"); } # Use ATS4 test plan.
	copy($epoc . "data/z/smoketest/smoketest_agenda.ini",				"temp/smoketest/general/smoketest_agenda.ini");
	copy($epoc . "data/z/smoketest/smoketest_agenda.script",			"temp/smoketest/general/smoketest_agenda.script");
	copy($epoc . "data/z/smoketest/smoketest_apploader.ini",			"temp/smoketest/general/smoketest_apploader.ini");
	copy($epoc . "data/z/smoketest/smoketest_apploader.script",			"temp/smoketest/general/smoketest_apploader.script");
	copy($epoc . "data/z/smoketest/smoketest_contacts.ini",				"temp/smoketest/general/smoketest_contacts.ini");
	copy($epoc . "data/z/smoketest/smoketest_contacts.script",			"temp/smoketest/general/smoketest_contacts.script");
	copy($epoc . "data/z/smoketest/smoketest_createsmsaccount.script",	"temp/smoketest/general/smoketest_createsmsaccount.script");
	copy($epoc . "data/z/smoketest/smoketest_mess.ini",					"temp/smoketest/general/smoketest_mess.ini");
	copy($epoc . "data/z/smoketest/smoketest_mess.script",				"temp/smoketest/general/smoketest_mess.script");
	copy($epoc . "data/z/smoketest/smoketest_phone.script",				"temp/smoketest/general/smoketest_phone.script");
	copy($epoc . "data/z/smoketest/smoketest_syncmlapp.script",			"temp/smoketest/general/smoketest_syncmlapp.script");
	copy($epoc . "data/z/smoketest/smoketest_system.script",			"temp/smoketest/general/smoketest_system.script");
	copy($epoc . "data/z/smoketest/smoketest_timew.ini",				"temp/smoketest/general/smoketest_timew.ini");
	copy($epoc . "data/z/smoketest/smoketest_timew.script",				"temp/smoketest/general/smoketest_timew.script");
	copy($epoc . "data/z/smoketest/smoketest_email.ini",				"temp/smoketest/general/smoketest_email.ini");
	copy($epoc . "data/z/smoketest/smoketest_email.script",				"temp/smoketest/general/smoketest_email.script");
	copy($epoc . "data/z/smoketest/smoketest_lbs.ini",					"temp/smoketest/general/smoketest_lbs.ini");
	copy($epoc . "data/z/smoketest/smoketest_lbs.script",				"temp/smoketest/general/smoketest_lbs.script");
	copy($epoc . "data/z/smoketest/smoketest_language.script",			"temp/smoketest/general/smoketest_language.script");
	copy($epoc . "data/z/smoketest/smoketest_bitmap.ini",				"temp/smoketest/general/smoketest_bitmap.ini");
	copy($epoc . "data/z/smoketest/smoketest_bitmap.script",			"temp/smoketest/general/smoketest_bitmap.script");
	copy($epoc . "data/z/smoketest/smoketest_http.ini",					"temp/smoketest/general/smoketest_http.ini");
	copy($epoc . "data/z/smoketest/smoketest_http.script",				"temp/smoketest/general/smoketest_http.script");
	copy($epoc . "data/z/smoketest/sms/message1.txt",					"temp/smoketest/sms/general/message1.txt");
	copy($epoc . "data/z/smoketest/MachineName.txt",					"temp/smoketest/general/MachineName.txt");
	copy($epoc . "data/z/smoketest/emailmessage/20KBody.txt",			"temp/smoketest/emailmessage/general/20KBody.txt");
	copy($epoc . "data/z/smoketest/emailsettings/popsettings_test908.txt",	"temp/smoketest/emailsettings/general/popsettings_test908.txt");
	copy($epoc . "data/z/private/10003a3f/apps/openservice1a.rsc",		"temp/smoketest/10003a3f/general/openservice1a.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/openservice1b.rsc",		"temp/smoketest/10003a3f/general/openservice1b.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp2_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp2_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp3_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp3_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp4_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp4_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp6_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp6_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp7_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp7_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/serverapp_reg.rsc",		"temp/smoketest/10003a3f/general/serverapp_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/t_dataprioritysystem3_reg.rsc",	"temp/smoketest/10003a3f/general/t_dataprioritysystem3_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/t_groupname_reg.rsc",	"temp/smoketest/10003a3f/general/t_groupname_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/t_groupnamever1_reg.rsc","temp/smoketest/10003a3f/general/t_groupnamever1_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/t_groupnamever2_reg.rsc","temp/smoketest/10003a3f/general/t_groupnamever2_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/tctrlpnlapp_reg.rsc",	"temp/smoketest/10003a3f/general/tctrlpnlapp_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/testtrustedpriorityapp1_reg.rsc","temp/smoketest/10003a3f/general/testtrustedpriorityapp1_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/apps/testtrustedpriorityapp2_reg.rsc","temp/smoketest/10003a3f/general/testtrustedpriorityapp2_reg.rsc");
	copy($epoc . "data/z/resource/apps/customisedefaulticonapp_loc.rsc","temp/smoketest/resource/general/customisedefaulticonapp_loc.rsc");
	copy($epoc . "data/z/resource/apps/default_app_icon.m02",			"temp/smoketest/resource/general/default_app_icon.m02");
	copy($epoc . "data/z/resource/apps/serverapp_loc.rsc",				"temp/smoketest/resource/general/serverapp_loc.rsc");
	copy($epoc . "data/z/resource/apps/t_groupname_loc.rsc",			"temp/smoketest/resource/general/t_groupname_loc.rsc");
	copy($epoc . "data/z/resource/apps/t_groupnamever1_loc.rsc",		"temp/smoketest/resource/general/t_groupnamever1_loc.rsc");
	copy($epoc . "data/z/resource/apps/t_groupnamever2_loc.rsc",		"temp/smoketest/resource/general/t_groupnamever2_loc.rsc");
	copy($epoc . "data/z/resource/apps/tstapp02.m02",					"temp/smoketest/resource/general/tstapp02.m02");
	copy($epoc . "data/z/resource/apps/tstapp.mbm",						"temp/smoketest/resource/general/tstapp.mbm");
	copy($epoc . "data/z/resource/apps/tstapp.rsc",						"temp/smoketest/resource/general/tstapp.rsc");
	copy($epoc . "data/z/resource/apps/tstapp_loc.r01",					"temp/smoketest/resource/general/tstapp_loc.r01");
	copy($epoc . "data/z/resource/apps/tstapp_loc.r02",					"temp/smoketest/resource/general/tstapp_loc.r02");
	copy($epoc . "data/z/resource/apps/tstapp_loc.r03",					"temp/smoketest/resource/general/tstapp_loc.r03");
	copy($epoc . "data/z/resource/apps/tstapp_loc.r04",					"temp/smoketest/resource/general/tstapp_loc.r04");
	copy($epoc . "data/z/resource/apps/tstapp_loc.r05",					"temp/smoketest/resource/general/tstapp_loc.r05");
	copy($epoc . "data/z/resource/apps/tstapp_loc.rsc",					"temp/smoketest/resource/general/tstapp_loc.rsc");
	copy($epoc . "data/z/resource/apps/tstappview",						"temp/smoketest/resource/general/tstappview");
	copy($epoc . "data/z/resource/apps/tstappview01.m01",				"temp/smoketest/resource/general/tstappview01.m01");
	copy($epoc . "data/z/resource/apps/tstappview01.m02",				"temp/smoketest/resource/general/tstappview01.m02");
	copy($epoc . "data/z/resource/apps/tstappview02.k",					"temp/smoketest/resource/general/tstappview02.k");
	copy($epoc . "data/z/private/10003a3f/import/apps/customisedefaulticonapp_reg.rsc",	"temp/smoketest/localisation/general/customisedefaulticonapp_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/import/apps/testuntrustedpriorityapp1_reg.rsc","temp/smoketest/localisation/general/testuntrustedpriorityapp1_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/import/apps/testuntrustedpriorityapp2_reg.rsc","temp/smoketest/localisation/general/testuntrustedpriorityapp2_reg.rsc");
	copy($epoc . "data/z/private/10003a3f/import/apps/tstapp_reg.rsc",	"temp/smoketest/localisation/general/tstapp_reg.rsc");
	copy($epoc . "data/z/resource/plugins/tstapp_embedded.rsc",			"temp/smoketest/localisation/general/tstapp_embedded.rsc");
	copy($epoc . "data/z/smoketest/bitmap/uibench_16bit.mbm",			"temp/smoketest/bitmap/general/uibench_16bit.mbm");
	copy($epoc . "data/z/delproxy.pl",									"temp/smoketest/ethernet/general/delproxy.pl");
	copy($epoc . "release/winscw/udeb/SmokeTestSecureFSclient.dll",		"temp/smoketest/winscw_udeb/SmokeTestSecureFSclient.dll");
	copy($epoc . "release/winscw/udeb/SmokeTestSecureFSserver.exe",		"temp/smoketest/winscw_udeb/SmokeTestSecureFSserver.exe");
	copy($epoc . "release/winscw/udeb/Smoketest_Agenda_Server.exe",		"temp/smoketest/winscw_udeb/Smoketest_Agenda_Server.exe");
	copy($epoc . "release/winscw/udeb/smoketest_apploader.exe",			"temp/smoketest/winscw_udeb/smoketest_apploader.exe");
	copy($epoc . "release/winscw/udeb/Smoketest_Contacts_Server.exe",	"temp/smoketest/winscw_udeb/Smoketest_Contacts_Server.exe");
	copy($epoc . "release/winscw/udeb/Smoketest_Mess_Server.exe",		"temp/smoketest/winscw_udeb/Smoketest_Mess_Server.exe");
	copy($epoc . "release/winscw/udeb/Smoketest_System.exe",			"temp/smoketest/winscw_udeb/Smoketest_System.exe");
	copy($epoc . "release/winscw/udeb/Smoketest_Timew_Server.exe",		"temp/smoketest/winscw_udeb/Smoketest_Timew_Server.exe");
	copy($epoc . "release/winscw/udeb/SmokeTest_Utils.dll",				"temp/smoketest/winscw_udeb/SmokeTest_Utils.dll");
	copy($epoc . "release/winscw/udeb/t_msgutilityserver.dll",			"temp/smoketest/winscw_udeb/t_msgutilityserver.dll");
	copy($epoc . "release/winscw/udeb/t_msgpopserver.exe",				"temp/smoketest/winscw_udeb/t_msgpopserver.exe");
	copy($epoc . "release/winscw/udeb/t_msgsmtpserver.exe",				"temp/smoketest/winscw_udeb/t_msgsmtpserver.exe");
	copy($epoc . "release/winscw/udeb/te_lbsapisuite.exe",				"temp/smoketest/winscw_udeb/te_lbsapisuite.exe");
	copy($epoc . "release/winscw/udeb/appfwk_test_AppUi.dll",			"temp/smoketest/winscw_udeb/appfwk_test_AppUi.dll");
	copy($epoc . "release/winscw/udeb/appfwk_test_utils.dll",			"temp/smoketest/winscw_udeb/appfwk_test_utils.dll");
	copy($epoc . "release/winscw/udeb/ticonloaderandiconarrayforleaks.dll",	"temp/smoketest/winscw_udeb/ticonloaderandiconarrayforleaks.dll");
	copy($epoc . "release/winscw/udeb/tstapp_embedded.dll",				"temp/smoketest/winscw_udeb/tstapp_embedded.dll");
	copy($epoc . "release/winscw/udeb/apparctestserver.exe",			"temp/smoketest/winscw_udeb/apparctestserver.exe");
	copy($epoc . "release/winscw/udeb/tstapp.exe",						"temp/smoketest/winscw_udeb/tstapp.exe");
	copy($epoc . "release/winscw/udeb/t_animplugin.dll",				"temp/smoketest/winscw_udeb/t_animplugin.dll");
	copy($epoc . "release/winscw/udeb/t_graphicswservapi.exe",			"temp/smoketest/winscw_udeb/t_graphicswservapi.exe");
	copy($epoc . "release/winscw/udeb/twsgraphicshare.exe",				"temp/smoketest/winscw_udeb/twsgraphicshare.exe");
	copy($epoc . "release/winscw/udeb/twsgraphicunshare.exe",			"temp/smoketest/winscw_udeb/twsgraphicunshare.exe");
	copy($epoc . "release/winscw/udeb/testhttpsuite.exe",				"temp/smoketest/winscw_udeb/testhttpsuite.exe");

	copy($epoc . "release/winscw/udeb/ProfileSmokeTestModule.dll",      "temp/smoketest/winscw_udeb/ProfileSmokeTestModule.dll");
	copy($epoc . "winscw/c/smoketest/ProfileSmokeTestModule.cfg",       "temp/smoketest/general/ProfileSmokeTestModule.cfg");
	copy($epoc . "release/winscw/udeb/AppInstallSmokeTestModule.dll",   "temp/smoketest/winscw_udeb/AppInstallSmokeTestModule.dll");
	copy($epoc . "winscw/c/smoketest/AppInstallSmokeTestModule.cfg",    "temp/smoketest/general/AppInstallSmokeTestModule.cfg");
	copy($epoc . "winscw/c/smoketest/appinstall/sistest_winscw.sisx",   "temp/smoketest/appinstall/general/sistest_winscw.sisx");
	copy($epoc . "release/winscw/udeb/TestNpdApi.dll",					"temp/smoketest/winscw_udeb/TestNpdApi.dll");
	copy($epoc . "data/c/smoketest/smoketest_notepad.cfg",				"temp/smoketest/general/smoketest_notepad.cfg");
	copy($epoc . "data/c/smoketest/ui_TestNpdApi.cfg",					"temp/smoketest/general/ui_TestNpdApi.cfg");
	copy($epoc . "release/winscw/udeb/testsdkskins.dll",				"temp/smoketest/winscw_udeb/testsdkskins.dll");
	copy($epoc . "data/c/smoketest/smoketest_uiresources.cfg",			"temp/smoketest/general/smoketest_uiresources.cfg");
	copy($epoc . "data/c/smoketest/ui_testsdkskinsu.cfg",				"temp/smoketest/general/ui_testsdkskinsu.cfg");
	copy($epoc . "data/c/smoketest/ui_testsdkskinswu.cfg",				"temp/smoketest/general/ui_testsdkskinswu.cfg");
	copy($epoc . "data/c/smoketest/wallpaper/symbian_01_320x480.jpg",	"temp/smoketest/wallpaper/general/symbian_01_320x480.jpg");
	copy($epoc . "release/winscw/udeb/InputMethodSmokeTestModule.dll",  "temp/smoketest/winscw_udeb/InputMethodSmokeTestModule.dll");
	copy($epoc . "winscw/c/smoketest/InputMethodSmokeTestModule.cfg",   "temp/smoketest/general/InputMethodSmokeTestModule.cfg");
	copy($epoc . "release/winscw/udeb/MediaMgmtSmokeTestModule.dll",	"temp/smoketest/winscw_udeb/MediaMgmtSmokeTestModule.dll");
	copy($epoc . "winscw/c/smoketest/UI_MediaMgmtSmokeTestModule.cfg",  "temp/smoketest/general/UI_MediaMgmtSmokeTestModule.cfg");
	copy($epoc . "winscw/c/smoketest/MediaMgmtSmokeTestModule.cfg",   	"temp/smoketest/general/MediaMgmtSmokeTestModule.cfg");
	copy($epoc . "data/c/smoketest/web/loadtest2.html",					"temp/smoketest/web/general/loadtest2.html");
	copy($epoc . "data/c/smoketest/web/img8.jpg",						"temp/smoketest/web/general/img8.jpg");
	copy($epoc . "winscw/c/smoketest/ui_browser_control_smoke.cfg",   	"temp/smoketest/general/ui_browser_control_smoke.cfg");
	copy($epoc . "release/winscw/udeb/BrCtlApiTest.dll",				"temp/smoketest/winscw_udeb/BrCtlApiTest.dll");
	
	copy($epoc . "release/winscw/udeb/tst_smoke_qapplication.exe",		"temp/smoketest/winscw_udeb/tst_smoke_qapplication.exe");
	copy($epoc . "release/winscw/udeb/tst_smoke_qwidget.exe",			"temp/smoketest/winscw_udeb/tst_smoke_qwidget.exe");
	copy($epoc . "release/winscw/udeb/tst_smoke_qtgestures.exe",		"temp/smoketest/winscw_udeb/tst_smoke_qtgestures.exe");
}
elsif ($target eq "SYBORG") { # Build ATS test drop for Syborg.
	mkpath "temp/smoketest/general";
	mkpath "temp/smoketest/emailmessage/general";
	mkpath "temp/smoketest/emailsettings/general";
	mkpath "temp/smoketest/web/general";

	if (lc($ats_version) eq "ats3") { copy("smoketest_syborg.xml",		"temp/test.xml"); } # Use ATS3 test plan.
	if (lc($ats_version) eq "ats4") { copy("ats4_smoketest_syborg.xml",	"temp/test.xml"); } # Use ATS4 test plan.
	copy($epoc . "data/z/smoketest/smoketest_apploader_syborg.ini",		"temp/smoketest/general/smoketest_apploader.ini");
	copy($epoc . "data/z/smoketest/MachineName.txt",					"temp/smoketest/general/MachineName.txt");
	copy($epoc . "data/z/smoketest/emailmessage/20KBody_syborg.txt",	"temp/smoketest/emailmessage/general/20KBody.txt");
	copy($epoc . "data/z/smoketest/emailsettings/popsettings_test908.txt",	"temp/smoketest/emailsettings/general/popsettings_test908.txt");
	copy($epoc . "winscw/c/smoketest/AppInstallSmokeTestModule_z.cfg",	"temp/smoketest/general/AppInstallSmokeTestModule.cfg");
	copy($epoc . "data/c/smoketest/ui_TestNpdApi.cfg",					"temp/smoketest/general/ui_TestNpdApi.cfg");
	copy($epoc . "data/c/smoketest/ui_testsdkskinsu.cfg",				"temp/smoketest/general/ui_testsdkskinsu.cfg");
	copy($epoc . "data/c/smoketest/ui_testsdkskinswu.cfg",				"temp/smoketest/general/ui_testsdkskinswu.cfg");
	copy($epoc . "winscw/c/smoketest/UI_MediaMgmtSmokeTestModule.cfg",  "temp/smoketest/general/UI_MediaMgmtSmokeTestModule.cfg");
	copy($epoc . "data/c/smoketest/web/loadtest2.html",					"temp/smoketest/web/general/loadtest2.html");
}

system("7z a -tzip smoketest.zip ./temp/*");

exit 0;

sub usage($)
{
    my $error = shift;
    my $fh = $error == 0 ? *STDOUT : *STDERR;
    print $fh "smoketest.pl\n" .
            "Specify the target\n" .
            "synopsis:\n" .
            "  smoketest.pl --help\n" .
            "  smoketest.pl [--target=TARGET] [--ats-version=VERSION]\n" .
            "options:\n" .
            "  --help                        Display this help and exit.\n" .
			"  --target=TARGET               TARGET is the target on which the smoketest will be run (WINSCW | SYBORG). If not specified WINSCW will be used.\n" .
            "  --ats-version=VERSION         VERSION is the version of ATS which will be used to run the smoketest (ATS3 | ATS4). If not specified ATS3 will be set by default.\n";
    exit $error;
}

sub help()
{
    usage(0);
}

sub usage_error()
{
    usage(1);
}
