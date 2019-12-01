
// fuzzylogicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fuzzylogic.h"
#include "fuzzylogicDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfuzzylogicDlg dialog


float suhuDingin[120], suhuSejuk[120], suhuHangat[120], suhuPanas[120], uSuhu[5];
float cuacaHujan[100], cuacaMendung[100], cuacaCerah[100],uCuaca[4];
float rpmLambat[1500], rpmSedang[1500], rpmCepat[1500],rpmAll[1500];
float lambat[5], sedang[5], cepat[5], terlambat = 0, tersedang = 0, tercepat = 0;
float hasilAkhir;
float atas, bawah;


CfuzzylogicDlg::CfuzzylogicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FUZZYLOGIC_DIALOG, pParent)
	//, m_chart_temperatur(0)
	//, m_chart_cuaca(0)
	//, m_chart_rpm(0)
	, m_setTemp(_T(""))
	, m_setCuaca(_T(""))
	, m_uTemp(_T(""))
	, m_uCuaca(_T(""))
	, m_ruleMin(_T(""))
	, m_maxRule(_T(""))
	//, m_chart_out(0)
	, m_rpmHasil(_T(""))
	, m_classing(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfuzzylogicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart_temperatur);
	DDX_Control(pDX, IDC_TCHART2, m_chart_cuaca);
	DDX_Control(pDX, IDC_TCHART3, m_chart_rpm);
	DDX_Text(pDX, IDC_EDIT1, m_setTemp);
	DDX_Text(pDX, IDC_EDIT2, m_setCuaca);
	DDX_Text(pDX, IDC_EDIT3, m_uTemp);
	DDX_Text(pDX, IDC_EDIT4, m_uCuaca);
	DDX_Text(pDX, IDC_EDIT5, m_ruleMin);
	DDX_Text(pDX, IDC_EDIT6, m_maxRule);
	DDX_Control(pDX, IDC_TCHART4, m_chart_out);
	DDX_Text(pDX, IDC_EDIT7, m_rpmHasil);
	DDX_Text(pDX, IDC_EDIT8, m_classing);
}

BEGIN_MESSAGE_MAP(CfuzzylogicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CfuzzylogicDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT3, &CfuzzylogicDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, &CfuzzylogicDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CfuzzylogicDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CfuzzylogicDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT6, &CfuzzylogicDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CfuzzylogicDlg::OnEnChangeEdit7)
	ON_BN_CLICKED(IDC_BUTTON4, &CfuzzylogicDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CfuzzylogicDlg message handlers

BOOL CfuzzylogicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//suhu Dingin
	for (int i = 0; i <= 30; i++) {
		suhuDingin[i] = 1.0;
	}
	for (int i = 31; i<= 50; i++) {
		suhuDingin[i] = ((50.0 - i) / (50.0 - 30.0));
	}
	for (int i = 50; i <= 120; i++) {
		suhuDingin[i] = 0;
	}
	for (int i = 0; i <= 120; i++) {
		((CSeries)(m_chart_temperatur.get_Series(0))).AddXY(i, suhuDingin[i], NULL, 536870912);
	}

	//suhu Sejuk
	for (int i = 0; i <= 30; i++) {
		suhuSejuk[i] = 0;
	}
	for (int i = 31; i <= 50; i++) {
		suhuSejuk[i] = ((i - 30.0) / (50.0 - 30.0));
	}
	for (int i = 51; i <= 70; i++) {
		suhuSejuk[i] = ((70.0 - i) / (70.0 - 50.0));
	}
	for (int i = 71; i <= 120; i++) {
		suhuSejuk[i] = 0;
	}
	for (int i = 0; i <= 120; i++) {
		((CSeries)(m_chart_temperatur.get_Series(1))).AddXY(i, suhuSejuk[i], NULL, 536870912);
	}

	//suhu Hangat
	for (int i = 0; i <= 50; i++) {
		suhuHangat[i] = 0;
	}
	for (int i = 51; i <= 70; i++) {
		suhuHangat[i] = ((i - 50.0) / (70.0 - 50.0));
	}
	for (int i = 71; i <= 90; i++) {
		suhuHangat[i] = ((90.0 - i) / (90.0 - 70.0));
	}
	for (int i = 91; i <= 120; i++) {
		suhuHangat[i] = 0;
	}
	for (int i = 0; i <= 100; i++) {
		((CSeries)(m_chart_temperatur.get_Series(2))).AddXY(i, suhuHangat[i], NULL, 536870912);
	}

	//suhu Panas
	for (int i = 0; i <= 70; i++) {
		suhuPanas[i] = 0;
	}
	for (int i = 71; i <= 90; i++) {
		suhuPanas[i] = ((i - 70.0) / (90.0 - 70.0));
	}
	
	for (int i = 91; i <= 120; i++) {
		suhuPanas[i] = 1;
	}
	for (int i = 0; i <= 120; i++) {
		((CSeries)(m_chart_temperatur.get_Series(3))).AddXY(i, suhuPanas[i], NULL, 536870912);
	}


	///CUACA
	//cuaca Hujan
	for (int i = 0; i <= 20; i++) {
		cuacaHujan[i] = 1.0;
	}
	for (int i = 21; i <= 50; i++) {
		cuacaHujan[i] = ((50.0 - i) / (50.0 - 20.0));
	}
	for (int i = 50; i <= 100; i++) {
		cuacaHujan[i] = 0;
	}
	for (int i = 0; i <= 100; i++) {
		((CSeries)(m_chart_cuaca.get_Series(0))).AddXY(i, cuacaHujan[i], NULL, 536870912);
	}
	
	//cuaca Mendung
	for (int i = 0; i <= 40; i++) {
		cuacaMendung[i] = 0;
	}
	for (int i = 41; i <= 55; i++) {
		cuacaMendung[i] = ((i - 40.0) / (55.0 - 40.0));
	}
	for (int i = 56; i <= 70; i++) {
		cuacaMendung[i] = ((70.0 - i) / (70.0 - 55.0));
	}
	for (int i = 71; i <= 100; i++) {
		cuacaMendung[i] = 0;
	}
	for (int i = 0; i <= 100; i++) {
		((CSeries)(m_chart_cuaca.get_Series(1))).AddXY(i, cuacaMendung[i], NULL, 536870912);
	}
	
	

	//Cuaca Cerah
	for (int i = 0; i <= 60; i++) {
		cuacaCerah[i] = 0;
	}
	for (int i = 61; i <= 80; i++) {
		cuacaCerah[i] = ((i - 60.0) / (80.0 - 60.0));
	}

	for (int i = 81; i <= 100; i++) {
		cuacaCerah[i] = 1;
	}
	for (int i = 0; i <= 100; i++) {
		((CSeries)(m_chart_cuaca.get_Series(2))).AddXY(i, cuacaCerah[i], NULL, 536870912);
	}


	///Output RPM
	//rpmLambat
	for (int i = 0; i <= 400; i++) {
		rpmLambat[i] = 1.0;
	}
	for (int i = 401; i <= 500; i++) {
		rpmLambat[i] = ((500.0 - i) / (500.0 - 400.0));
	}
	for (int i = 501; i <= 1500; i++) {
		rpmLambat[i] = 0;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(0))).AddXY(i, rpmLambat[i], NULL, 536870912);
	}

	//rpmSedang
	for (int i = 0; i <= 450; i++) {
		rpmSedang[i] = 0;
	}
	for (int i = 451; i <= 550; i++) {
		rpmSedang[i] = ((i - 450.0) / (550.0 - 450.0));
	}
	for (int i = 551; i <= 950; i++) {
		rpmSedang[i] = 1.0;
	}
	for (int i = 951; i <= 1050; i++) {
		rpmSedang[i] = ((1050.0 - i) / (1050.0 - 950.0));
	}
	for (int i = 1051; i <= 1500; i++) {
		rpmSedang[i] = 0.0;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(1))).AddXY(i, rpmSedang[i], NULL, 536870912);
	}

	

	//rpmCepat
	for (int i = 0; i <= 900; i++) {
		rpmCepat[i] = 0;
	}
	for (int i = 901; i <= 1100; i++) {
		rpmCepat[i] = ((i - 900.0) / (1100.0 - 900.0));
	}

	for (int i = 1101; i <= 1500; i++) {
		rpmCepat[i] = 1;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(2))).AddXY(i, rpmCepat[i], NULL, 536870912);
	}

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CfuzzylogicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CfuzzylogicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CfuzzylogicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);

}





void CfuzzylogicDlg::OnBnClickedButton1()
{
	CString convDingin, convSejuk, convHangat,convPanas;
	CString convHujan, convMendung, convCerah;
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	uSuhu[0] = suhuDingin[_ttoi(m_setTemp)];
	m_uTemp = " u(";
	m_uTemp += m_setTemp;
	m_uTemp += ") [DINGIN] : ";
	convDingin.Format(_T("%.2f"), uSuhu[0]);
	m_uTemp += convDingin;

	uSuhu[1] = suhuSejuk[_ttoi(m_setTemp)];
	m_uTemp += "\r\n u(";
	m_uTemp += m_setTemp;
	m_uTemp += ") [SEJUK] : ";
	convSejuk.Format(_T("%.2f"), uSuhu[1]);
	m_uTemp += convSejuk;

	uSuhu[2] = suhuHangat[_ttoi(m_setTemp)];
	m_uTemp += "\r\n u(";
	m_uTemp += m_setTemp;
	m_uTemp += ") [HANGAT] : ";
	convHangat.Format(_T("%.2f"), uSuhu[2]);
	m_uTemp += convHangat;

	uSuhu[3] = suhuPanas[_ttoi(m_setTemp)];
	m_uTemp += "\r\n u(";
	m_uTemp += m_setTemp;
	m_uTemp += ") [PANAS] : ";
	convPanas.Format(_T("%.2f"), uSuhu[3]);
	m_uTemp += convPanas;


	//cuaca
	uCuaca[0] = cuacaHujan[_ttoi(m_setCuaca)];
	m_uCuaca = " u(";
	m_uCuaca += m_setCuaca;
	m_uCuaca += ") [HUJAN] : ";
	convHujan.Format(_T("%.2f"), uCuaca[0]);
	m_uCuaca += convHujan;

	uCuaca[1] = cuacaMendung[_ttoi(m_setCuaca)];
	m_uCuaca += "\r\n u(";
	m_uCuaca += m_setCuaca;
	m_uCuaca += ") [MENDUNG] : ";
	convMendung.Format(_T("%.2f"), uCuaca[1]);
	m_uCuaca += convMendung;

	uCuaca[2] = cuacaCerah[_ttoi(m_setCuaca)];
	m_uCuaca += "\r\n u(";
	m_uCuaca += m_setCuaca;
	m_uCuaca += ") [CERAH] : ";
	convCerah.Format(_T("%.2f"), uCuaca[2]);
	m_uCuaca += convCerah;

	
	UpdateData(FALSE);

	



}


void CfuzzylogicDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CfuzzylogicDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CfuzzylogicDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString lambatStr[5], sedangStr[5], cepatStr[5], terlambatStr, tersedangStr, tercepatStr;
	//lambat 1
	if (uSuhu[0] >= uCuaca[0]) {
		lambat[0] = uCuaca[0];
	}
	else {
		lambat[0] = uSuhu[0];
	}
	//lambat2
	if (uSuhu[0] >= uCuaca[1]) {
		lambat[1] = uCuaca[1];
	}
	else {
		lambat[1] = uSuhu[0];
	}
	//lambat4
	if (uSuhu[1] >= uCuaca[0]) {
		lambat[2] = uCuaca[0];
	}
	else {
		lambat[2] = uSuhu[1];
	}
	
	//sedang
	//3
	if (uSuhu[0] >= uCuaca[2]) {
		sedang[0] = uCuaca[2];
	}
	else {
		sedang[0] = uSuhu[0];
	}
	//5
	if (uSuhu[1] >= uCuaca[1]) {
		sedang[1] = uCuaca[1];
	}
	else {
		sedang[1] = uSuhu[1];
	}

	//6
	if (uSuhu[1] >= uCuaca[2]) {
		sedang[2] = uCuaca[2];
	}
	else {
		sedang[2] = uSuhu[1];
	}
	//7
	if (uSuhu[2] >= uCuaca[0]) {
		sedang[3] = uCuaca[0];
	}
	else {
		sedang[3] = uSuhu[2];
	}


	//cepat
	//8
	if (uSuhu[2] >= uCuaca[1]) {
		cepat[0] = uCuaca[1];
	}
	else {
		cepat[0] = uSuhu[2];
	}
	//9
	if (uSuhu[2] >= uCuaca[2]) {
		cepat[1] = uCuaca[2];
	}
	else {
		cepat[1] = uSuhu[2];
	}

	//10
	if (uSuhu[3] >= uCuaca[0]) {
		cepat[2] = uCuaca[0];
	}
	else {
		cepat[2] = uSuhu[3];
	}
	//11
	if (uSuhu[3] >= uCuaca[1]) {
		cepat[3] = uCuaca[1];
	}
	else {
		cepat[3] = uSuhu[3];
	}

	//12
	if (uSuhu[3] >= uCuaca[2]) {
		cepat[4] = uCuaca[2];
	}
	else {
		cepat[4] = uSuhu[3];
	}


	//max rule
	for (int i = 0; i<5; i++)
	{
		if (lambat[i]>terlambat)
			terlambat = lambat[i];
		if (sedang[i]>tersedang)
			tersedang = sedang[i];
		if (cepat[i]>tercepat)
			tercepat = cepat[i];
	}
	




	UpdateData(TRUE);
	//lambat
	m_ruleMin = "Lambat : \r\n [1] =>";
	lambatStr[0].Format(_T("%.2f"),lambat[0]);
	m_ruleMin += lambatStr[0];

	m_ruleMin += " \r\n [2] =>";
	lambatStr[1].Format(_T("%.2f"), lambat[1]);
	m_ruleMin += lambatStr[1];

	m_ruleMin += " \r\n [4] =>";
	lambatStr[2].Format(_T("%.2f"), lambat[2]);
	m_ruleMin += lambatStr[2];
	
	
	//sedang
	m_ruleMin += "\r\nSedang : \r\n [3] =>";
	sedangStr[0].Format(_T("%.2f"), sedang[0]);
	m_ruleMin += sedangStr[0];

	m_ruleMin += " \r\n [5] =>";
	sedangStr[1].Format(_T("%.2f"), sedang[1]);
	m_ruleMin += sedangStr[1];

	m_ruleMin += " \r\n [6] =>";
	sedangStr[2].Format(_T("%.2f"), sedang[2]);
	m_ruleMin += sedangStr[2];

	m_ruleMin += " \r\n [7] =>";
	sedangStr[3].Format(_T("%.2f"), sedang[3]);
	m_ruleMin += sedangStr[3];


	//CEPAT
	m_ruleMin += "\r\nCepat : \r\n [8] =>";
	cepatStr[0].Format(_T("%.2f"), cepat[0]);
	m_ruleMin += cepatStr[0];

	m_ruleMin += " \r\n [9] =>";
	cepatStr[1].Format(_T("%.2f"), cepat[1]);
	m_ruleMin += cepatStr[1];

	m_ruleMin += " \r\n [10] =>";
	cepatStr[2].Format(_T("%.2f"), cepat[2]);
	m_ruleMin += cepatStr[2];

	m_ruleMin += " \r\n [11] =>";
	cepatStr[3].Format(_T("%.2f"), cepat[3]);
	m_ruleMin += cepatStr[3];

	m_ruleMin += " \r\n [12] =>";
	cepatStr[4].Format(_T("%.2f"), cepat[4]);
	m_ruleMin += cepatStr[4];


	m_maxRule = "Lambat => ";
	terlambatStr.Format(_T("%.2f"), terlambat);
	m_maxRule += terlambatStr;

	m_maxRule += "\r\nSedang => ";
	tersedangStr.Format(_T("%.2f"), tersedang);
	m_maxRule += tersedangStr;

	m_maxRule += "\r\nCepat => ";
	tercepatStr.Format(_T("%.2f"), tercepat);
	m_maxRule += tercepatStr;


	UpdateData(FALSE);

}


void CfuzzylogicDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i <= 1500; i++) {
		if (rpmLambat[i] >= terlambat) {
			rpmLambat[i] = terlambat;
		}
		if (rpmSedang[i] >= tersedang) {
			rpmSedang[i] = tersedang;
		}
		if (rpmCepat[i] >= tercepat) {
			rpmCepat[i] = tercepat;
		}

	}

	for (int i = 0; i <= 750; i++) {
		if (rpmLambat[i] >= rpmSedang[i]) {
			rpmAll[i] = rpmLambat[i];
		}
		else {
			rpmAll[i] = rpmSedang[i];
		}
		((CSeries)(m_chart_out.get_Series(0))).AddXY(i, rpmAll[i], NULL, 536870912);

	}

	for (int i = 750; i <= 1500; i++) {

		if (rpmSedang[i] >= rpmCepat[i]) {
			rpmAll[i] = rpmSedang[i];
		}
		else {
			rpmAll[i] = rpmCepat[i];
		}

	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_out.get_Series(0))).AddXY(i, rpmAll[i], NULL, 536870912);
	}

	//Weight Avg Method
	//hasilAkhir = ((250 * terlambat) + (750 * tersedang) + (1200 * tercepat)) / (terlambat + tersedang + tercepat);

	//COG METHOD
	for (int i = 0; i <= 1500; i++) {
		
		atas += i * rpmAll[i];
		bawah += rpmAll[i];
		hasilAkhir = atas / bawah;
	}

	if (rpmLambat[(int)hasilAkhir] >= rpmSedang[(int)hasilAkhir] && rpmLambat[(int)hasilAkhir] >= rpmCepat[(int)hasilAkhir]) {
		UpdateData(TRUE);
		m_classing = "LAMBAT";
		UpdateData(FALSE);
	}else if(rpmSedang[(int)hasilAkhir] >= rpmLambat[(int)hasilAkhir] && rpmSedang[(int)hasilAkhir] >= rpmCepat[(int)hasilAkhir]){
		UpdateData(TRUE);
		m_classing = "SEDANG";
		UpdateData(FALSE);
	}
	else if (rpmCepat[(int)hasilAkhir] >= rpmLambat[(int)hasilAkhir] && rpmCepat[(int)hasilAkhir] >= rpmSedang[(int)hasilAkhir]) {
		UpdateData(TRUE);
		m_classing = "CEPAT";
		UpdateData(FALSE);
	}

	UpdateData(TRUE);
	m_rpmHasil.Format(_T("%.2f"), hasilAkhir);
	UpdateData(FALSE);
}


void CfuzzylogicDlg::OnEnChangeEdit6()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CfuzzylogicDlg::OnEnChangeEdit7()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CfuzzylogicDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here

	uSuhu[5]= NULL;
	uCuaca[4] = NULL;
	rpmLambat[1500] = NULL;
	rpmSedang[1500] = NULL;
	rpmCepat[1500] = NULL;
	rpmAll[1500] = NULL;
	lambat[5] = NULL;
	sedang[5] = NULL;
	cepat[5];
	terlambat = 0;
	tersedang = 0;
	tercepat = 0;
	hasilAkhir= 0;


	((CSeries)(m_chart_rpm.get_Series(0))).Clear();
	((CSeries)(m_chart_rpm.get_Series(1))).Clear();
	((CSeries)(m_chart_rpm.get_Series(2))).Clear();

	((CSeries)(m_chart_out.get_Series(0))).Clear();

	//rpmLambat
	for (int i = 0; i <= 400; i++) {
		rpmLambat[i] = 1.0;
	}
	for (int i = 401; i <= 500; i++) {
		rpmLambat[i] = ((500.0 - i) / (500.0 - 400.0));
	}
	for (int i = 501; i <= 1500; i++) {
		rpmLambat[i] = 0;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(0))).AddXY(i, rpmLambat[i], NULL, 536870912);
	}

	//rpmSedang
	for (int i = 0; i <= 450; i++) {
		rpmSedang[i] = 0;
	}
	for (int i = 451; i <= 550; i++) {
		rpmSedang[i] = ((i - 450.0) / (550.0 - 450.0));
	}
	for (int i = 551; i <= 950; i++) {
		rpmSedang[i] = 1.0;
	}
	for (int i = 951; i <= 1050; i++) {
		rpmSedang[i] = ((1050.0 - i) / (1050.0 - 950.0));
	}
	for (int i = 1051; i <= 1500; i++) {
		rpmSedang[i] = 0.0;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(1))).AddXY(i, rpmSedang[i], NULL, 536870912);
	}



	//rpmCepat
	for (int i = 0; i <= 900; i++) {
		rpmCepat[i] = 0;
	}
	for (int i = 901; i <= 1100; i++) {
		rpmCepat[i] = ((i - 900.0) / (1100.0 - 900.0));
	}

	for (int i = 1101; i <= 1500; i++) {
		rpmCepat[i] = 1;
	}
	for (int i = 0; i <= 1500; i++) {
		((CSeries)(m_chart_rpm.get_Series(2))).AddXY(i, rpmCepat[i], NULL, 536870912);
	}
	UpdateData(TRUE);
	m_uCuaca = " ";
	m_uTemp = " ";
	m_setCuaca = " ";
	m_setTemp = " ";
	m_ruleMin = " ";
	m_maxRule = " ";
	m_rpmHasil = " ";
	atas = 0;
	bawah = 0;
	UpdateData(FALSE);
}
