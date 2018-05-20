object FmWiseIP: TFmWiseIP
  Left = 236
  Top = 92
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsDialog
  Caption = #28165#20998#20449#24687
  ClientHeight = 526
  ClientWidth = 733
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clNavy
  Font.Height = -16
  Font.Name = #23435#20307
  Font.Style = [fsBold]
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Panel2: TPanel
    Left = 249
    Top = 345
    Width = 484
    Height = 181
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 2
    TabOrder = 1
    object GroupBox1: TGroupBox
      Left = 2
      Top = 2
      Width = 480
      Height = 101
      Align = alTop
      Caption = #28165#20998#30721#35782#21035#32467#26524
      Enabled = False
      Font.Charset = GB2312_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = #26032#23435#20307
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label1: TLabel
        Left = 20
        Top = 29
        Width = 69
        Height = 21
        Alignment = taCenter
        AutoSize = False
        Caption = #30913#30721#35782#21035
        Layout = tlCenter
      end
      object Label2: TLabel
        Left = 20
        Top = 63
        Width = 69
        Height = 21
        Alignment = taCenter
        AutoSize = False
        Caption = #22270#20687#35782#21035
        Layout = tlCenter
      end
      object txtMicrCode: TEdit
        Left = 96
        Top = 29
        Width = 369
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
        ReadOnly = True
        TabOrder = 0
      end
      object txtOcrCode: TEdit
        Left = 96
        Top = 63
        Width = 369
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
        ReadOnly = True
        TabOrder = 1
      end
    end
    object btnOk: TBitBtn
      Left = 317
      Top = 128
      Width = 129
      Height = 29
      Caption = #30830' '#23450
      TabOrder = 1
      OnClick = btnOkClick
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333330000333333333333333333333333F33333333333
        00003333344333333333333333388F3333333333000033334224333333333333
        338338F3333333330000333422224333333333333833338F3333333300003342
        222224333333333383333338F3333333000034222A22224333333338F338F333
        8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
        33333338F83338F338F33333000033A33333A222433333338333338F338F3333
        0000333333333A222433333333333338F338F33300003333333333A222433333
        333333338F338F33000033333333333A222433333333333338F338F300003333
        33333333A222433333333333338F338F00003333333333333A22433333333333
        3338F38F000033333333333333A223333333333333338F830000333333333333
        333A333333333333333338330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
      Spacing = 16
    end
    object btnHistQuery: TBitBtn
      Left = 94
      Top = 128
      Width = 129
      Height = 29
      Cancel = True
      Caption = #20154#34892#25968#25454
      TabOrder = 2
      OnClick = btnHistQueryClick
      NumGlyphs = 2
      Spacing = 16
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 733
    Height = 345
    Align = alTop
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 0
    object VchScrollBox: TScrollBox
      Left = 1
      Top = 1
      Width = 731
      Height = 343
      Align = alClient
      TabOrder = 0
      object VchImage: TImage
        Left = 0
        Top = 0
        Width = 721
        Height = 329
        Stretch = True
      end
      object TimeLeft: TLabel
        Left = 40
        Top = 32
        Width = 120
        Height = 21
        Caption = '90        '
        Font.Charset = GB2312_CHARSET
        Font.Color = clRed
        Font.Height = -21
        Font.Name = #23435#20307
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 345
    Width = 249
    Height = 181
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 2
    object Label3: TLabel
      Left = 12
      Top = 24
      Width = 77
      Height = 21
      AutoSize = False
      Caption = #20973#35777#21495#30721
      Layout = tlCenter
    end
    object Label4: TLabel
      Left = 12
      Top = 52
      Width = 77
      Height = 21
      AutoSize = False
      Caption = #20132#25442#21495#30721
      Layout = tlCenter
    end
    object Label5: TLabel
      Left = 12
      Top = 80
      Width = 77
      Height = 21
      AutoSize = False
      Caption = #24080'    '#21495
      Layout = tlCenter
    end
    object Label6: TLabel
      Left = 12
      Top = 108
      Width = 77
      Height = 21
      AutoSize = False
      Caption = #31181#31867#20195#21495
      Layout = tlCenter
    end
    object Label7: TLabel
      Left = 12
      Top = 136
      Width = 77
      Height = 21
      AutoSize = False
      Caption = #37329'    '#39069
      Layout = tlCenter
    end
    object Bevel1: TBevel
      Left = 8
      Top = 9
      Width = 233
      Height = 2
    end
    object Label8: TLabel
      Left = 16
      Top = 6
      Width = 48
      Height = 12
      Alignment = taRightJustify
      Caption = #31080#25454#20449#24687
      Enabled = False
      Font.Charset = GB2312_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = #26032#23435#20307
      Font.Style = []
      ParentFont = False
      Layout = tlCenter
    end
    object txtVchNo: TEdit
      Left = 96
      Top = 24
      Width = 145
      Height = 24
      ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
      MaxLength = 6
      TabOrder = 0
    end
    object txtNetNo: TEdit
      Left = 96
      Top = 52
      Width = 145
      Height = 24
      ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
      MaxLength = 9
      TabOrder = 1
    end
    object txtAccount: TEdit
      Left = 96
      Top = 80
      Width = 145
      Height = 24
      ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
      MaxLength = 12
      TabOrder = 2
    end
    object txtVchKind: TEdit
      Left = 96
      Top = 108
      Width = 145
      Height = 24
      ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
      MaxLength = 2
      TabOrder = 3
    end
    object txtMoney: TEdit
      Left = 96
      Top = 136
      Width = 145
      Height = 24
      ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
      MaxLength = 14
      TabOrder = 4
      OnChange = txtMoneyChange
      OnEnter = txtMoneyEnter
      OnKeyPress = txtMoneyKeyPress
    end
  end
  object TimerDie: TTimer
    Interval = 500
    OnTimer = TimerDieTimer
    Left = 512
    Top = 224
  end
end
