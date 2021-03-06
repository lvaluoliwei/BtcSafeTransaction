#include "stdafx.h"
#include "BtcSafeTransaction.h"
#include "SettingDlg.h"
#include "option.h"
#include "util.h"
#include <sdk/util/AppPath.hpp>
#include <boost/algorithm/string.hpp>
#include "lang_str.h"
//////////////////////////////////////////////////////////////////////////
UINT SettingDlg_DoModal()
{
	CSettingDlg dlg;
	return dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC( CSettingDlg, CDialog )

CSettingDlg::CSettingDlg( CWnd* pParent /*=NULL*/ )
	: CDialog( CSettingDlg::IDD, pParent )
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( CSettingDlg, CDialog )
	ON_BN_CLICKED( IDOK, &CSettingDlg::OnBnClickedOk )
	ON_BN_CLICKED( IDC_BUTTON_SET_SEL1, &CSettingDlg::OnBnClickedButtonSetSel1 )
	ON_BN_CLICKED( IDC_BUTTON_SET_SEL2, &CSettingDlg::OnBnClickedButtonSetSel2 )
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序


void CSettingDlg::OnBnClickedOk()
{
	g_pOption->Bin_DIR = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_BIN_DIR ) );
	g_pOption->DB_DIR = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_DB_DIR ) );
	g_pOption->IP = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_IP ) );
	g_pOption->Port = atoi( GetWindowStlText( GetDlgItem( IDC_EDIT_SET_PORT ) ).c_str() );
	g_pOption->LoginUser = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_USER ) );
	g_pOption->LoginPass = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_PASS ) );
	g_pOption->StartExtern = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_EXEC_PARAM ) );
	g_pOption->FileName_UIApp = GetWindowStlText( GetDlgItem( IDC_COMBO_SET_FNAME1 ) );
	g_pOption->FileName_ConsApp = GetWindowStlText( GetDlgItem( IDC_COMBO_SET_FNAME2 ) );
	g_pOption->UseUIApp = ( ( CButton*)( GetDlgItem( IDC_CHECK_IS_UI_APP ) ) )->GetCheck();

	g_pOption->PubAddrLabel = GetWindowStlText( GetDlgItem( IDC_EDIT_SET_PUBADDR_LABEL ) );
	g_pOption->DonateAuthor = ( ( CButton*)( GetDlgItem( IDC_CHECK_DONATE ) ) )->GetCheck();

	g_pOption->txfee = atof(GetWindowStlText(GetDlgItem(IDC_EDIT_TX_FEE)).c_str());

	boost::algorithm::trim( g_pOption->IP );
	boost::algorithm::trim( g_pOption->LoginUser );
	boost::algorithm::trim( g_pOption->LoginPass );
	boost::algorithm::trim( g_pOption->PubAddrLabel );
	boost::algorithm::trim( g_pOption->FileName_UIApp );
	boost::algorithm::trim( g_pOption->FileName_ConsApp );
	SaveMyAppOption();

	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

BOOL CSettingDlg::OnInitDialog()
{
	__super::OnInitDialog();

	SetDlgItemText( IDC_EDIT_SET_BIN_DIR, g_pOption->Bin_DIR.c_str() );
	SetDlgItemText( IDC_EDIT_SET_DB_DIR, g_pOption->DB_DIR.c_str() );
	SetDlgItemText( IDC_EDIT_SET_IP, g_pOption->IP.c_str() );
	SetDlgItemInt( IDC_EDIT_SET_PORT, g_pOption->Port );
	SetDlgItemText( IDC_EDIT_SET_USER, g_pOption->LoginUser.c_str() );
	SetDlgItemText( IDC_EDIT_SET_PASS, g_pOption->LoginPass.c_str() );

	( ( CComboBox*)GetDlgItem( IDC_COMBO_SET_FNAME1 ) )->AddString( "Bitcoin-qt.exe" );
	( ( CComboBox*)GetDlgItem( IDC_COMBO_SET_FNAME1 ) )->AddString( "Litecoin-qt.exe" );
	SetDlgItemText( IDC_COMBO_SET_FNAME1, g_pOption->FileName_UIApp.c_str() );
	( ( CComboBox*)GetDlgItem( IDC_COMBO_SET_FNAME2 ) )->AddString( "bitcoind.exe" );
	( ( CComboBox*)GetDlgItem( IDC_COMBO_SET_FNAME2 ) )->AddString( "litecoind.exe" );
	SetDlgItemText( IDC_COMBO_SET_FNAME2, g_pOption->FileName_ConsApp.c_str() );
	( ( CButton*)GetDlgItem( IDC_CHECK_IS_UI_APP ) )->SetCheck( g_pOption->UseUIApp );

	SetDlgItemText( IDC_EDIT_SET_EXEC_PARAM, g_pOption->StartExtern.c_str() );
	SetDlgItemText( IDC_EDIT_SET_PUBADDR_LABEL, g_pOption->PubAddrLabel.c_str() );
	( ( CButton*)GetDlgItem( IDC_CHECK_DONATE ) )->SetCheck( g_pOption->DonateAuthor );

	SetDlgItemText( IDC_EDIT_TX_FEE, amount2str(g_pOption->txfee).c_str() );

	return TRUE;
}

int CALLBACK DirectorySelect_Callback( HWND hwnd, UINT msg, LPARAM lp, LPARAM pData )
{
	if( msg == BFFM_INITIALIZED )
	{
		assert( pData );
		if( pData )
		{
			::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, ( LPARAM )pData );
		}
	}
	return 0;
}
std::string DirectorySelectDlg( HWND hParent, const TCHAR* title,  const TCHAR* old )
{

	std::string result;

	LPMALLOC pMalloc;
	if( ::SHGetMalloc( &pMalloc ) == NOERROR )
	{
		BROWSEINFO bi = {0};

		bi.hwndOwner = hParent;
		bi.lpszTitle = title;
		bi.ulFlags = 0;
		bi.lpfn = DirectorySelect_Callback;
		bi.lParam = ( LPARAM )old;

		TCHAR pszBuffer[MAX_PATH];
		if( LPITEMIDLIST pidl =::SHBrowseForFolder( &bi ) )
		{
			if( ::SHGetPathFromIDList( pidl, pszBuffer ) )
			{
				result = pszBuffer;
			}
			pMalloc->Free( pidl );
		}
		pMalloc->Release();
	}
	return result;
}


void CSettingDlg::OnBnClickedButtonSetSel1()
{
	std::string dir = GetFullFilePath( GetWindowStlText( GetDlgItem( IDC_EDIT_SET_BIN_DIR ) ) );
	dir = DirectorySelectDlg( *this, STR_SELECT_QT_DIR, dir.c_str() );
	if( dir.size() )
	{
		SetDlgItemText( IDC_EDIT_SET_BIN_DIR, dir.c_str() );
	}

}


void CSettingDlg::OnBnClickedButtonSetSel2()
{
	std::string dir = GetFullFilePath( GetWindowStlText( GetDlgItem( IDC_EDIT_SET_DB_DIR ) ) );
	dir = DirectorySelectDlg( *this, STR_SELECT_BB_DIR, dir.c_str() );
	if( dir.size() )
	{
		SetDlgItemText( IDC_EDIT_SET_DB_DIR, dir.c_str() );
	}
}
