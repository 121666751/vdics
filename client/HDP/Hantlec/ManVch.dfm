object ManVchAdd: TManVchAdd
  Left = 346
  Top = 148
  Width = 415
  Height = 377
  BorderIcons = [biSystemMenu]
  Caption = #20154#24037#34917#24405
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 64
    Top = 64
    Width = 297
    Height = 201
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 72
    Top = 88
    Width = 84
    Height = 16
    Caption = #20973'   '#35777'   '#21495'          '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 72
    Top = 120
    Width = 81
    Height = 16
    Caption = #20132'   '#25442'   '#21495'         '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 72
    Top = 152
    Width = 72
    Height = 16
    Caption = #36134'           '#21495'     '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 72
    Top = 192
    Width = 78
    Height = 16
    Caption = #20973#35777#31867#22411'          '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 72
    Top = 224
    Width = 75
    Height = 16
    Caption = #37329'          '#39069'       '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 128
    Top = 24
    Width = 208
    Height = 20
    Caption = #25903' '#31080' '#30913' '#30721' '#20449' '#24687'                       '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 312
    Top = 232
    Width = 36
    Height = 13
    Caption = #65288#20998#65289
  end
  object ok: TButton
    Left = 96
    Top = 280
    Width = 89
    Height = 33
    Caption = #30830#23450
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = okClick
  end
  object cancel: TButton
    Left = 256
    Top = 280
    Width = 89
    Height = 33
    Caption = #21462#28040
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 2
    ParentFont = False
    TabOrder = 1
  end
  object vchno: TEdit
    Left = 152
    Top = 88
    Width = 153
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 6
    TabOrder = 2
    OnKeyPress = vchnoKeyPress
  end
  object exchno: TEdit
    Left = 152
    Top = 120
    Width = 153
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 8
    TabOrder = 3
    OnKeyPress = exchnoKeyPress
  end
  object accno: TEdit
    Left = 152
    Top = 152
    Width = 153
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 12
    TabOrder = 4
    OnKeyPress = accnoKeyPress
  end
  object vchtype: TEdit
    Left = 152
    Top = 192
    Width = 153
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 2
    TabOrder = 5
    OnKeyPress = vchtypeKeyPress
  end
  object vchmoney: TEdit
    Left = 152
    Top = 224
    Width = 153
    Height = 21
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 10
    TabOrder = 6
    OnKeyPress = vchmoneyKeyPress
  end
end
