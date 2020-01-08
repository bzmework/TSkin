VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "换肤测试"
   ClientHeight    =   5145
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8805
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   343
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   587
   StartUpPosition =   1  '所有者中心
   WhatsThisButton =   -1  'True
   WhatsThisHelp   =   -1  'True
   Begin VB.CommandButton Command3 
      Caption         =   "窗口透明"
      Height          =   555
      Left            =   3810
      TabIndex        =   3
      Top             =   3180
      Width           =   2895
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Arial-8 Microsoft Visual Basic 微软"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   555
      Left            =   3810
      TabIndex        =   1
      Top             =   1560
      Width           =   2895
   End
   Begin VB.CommandButton Command1 
      Caption         =   "宋体-9 UnLoad Me 微软"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   555
      Left            =   3810
      TabIndex        =   0
      Top             =   720
      Width           =   2895
   End
   Begin VB.Label Label1 
      Caption         =   "Tahoma-8 Microsoft Visual Basic 微软"
      Height          =   375
      Left            =   3810
      TabIndex        =   2
      Top             =   2460
      Width           =   2895
   End
   Begin VB.Image Image1 
      Height          =   480
      Left            =   7950
      Picture         =   "frmMain.frx":0000
      Top             =   450
      Visible         =   0   'False
      Width           =   480
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private mclsSkin As TSkin.Skin
 
Private Sub Command1_Click()
    Unload Me
End Sub

Private Sub Command3_Click()
    Dim nAlpha As Byte
    
    nAlpha = Rnd * 255
    If nAlpha = 0 Then nAlpha = 10
    mclsSkin.Transparent nAlpha
    
End Sub

Private Sub Form_Load()
    
    Me.BackColor = RGB(207, 221, 232)
    
    Set mclsSkin = New TSkin.Skin
    
    '自定义设置
    mclsSkin.Alpha = 200
    mclsSkin.BorderWidth = 4
    mclsSkin.BackColor = RGB(255, 0, 0) ' RGB(207, 221, 232) '未用
    mclsSkin.GradientBeginColor = RGB(58, 26, 167)
    mclsSkin.GradientEndColor = RGB(255, 126, 0)
    mclsSkin.TitleBarHeight = 30
    mclsSkin.TitleColor = RGB(255, 0, 0)
    mclsSkin.CloseButtonColor = -1

    mclsSkin.ShadowColor = RGB(0, 0, 0)
    mclsSkin.ShadowDepth = 5
    mclsSkin.ShadowTransparency = 200
    mclsSkin.ShadowType = stFullShadow
     
    mclsSkin.ApplySkin Me.hWnd
     
    Me.Caption = "窗口:hello world" & Now
    Set Me.Icon = Image1.Picture
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    
    Set mclsSkin = Nothing
    
End Sub
