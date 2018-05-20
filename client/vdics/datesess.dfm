object FormDate: TFormDate
  Left = 364
  Top = 367
  Width = 419
  Height = 213
  Caption = #38598#20013#25552#22238
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 411
    Height = 121
    ActivePage = TabSheet1
    Align = alTop
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #20132#25442#25552#22238#21442#25968#35774#32622
      object GroupBox1: TGroupBox
        Left = 9
        Top = 6
        Width = 380
        Height = 75
        TabOrder = 0
        object Label3: TLabel
          Left = 202
          Top = 31
          Width = 73
          Height = 17
          AutoSize = False
          Caption = #25552#20837#22330#27425
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = #23435#20307
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 13
          Top = 32
          Width = 73
          Height = 17
          AutoSize = False
          Caption = #25552#20837#26085#26399
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = #23435#20307
          Font.Style = []
          ParentFont = False
        end
        object txtDate: TDateTimePicker
          Left = 80
          Top = 27
          Width = 100
          Height = 21
          CalAlignment = dtaLeft
          Date = 38064.5587181713
          Time = 38064.5587181713
          DateFormat = dfShort
          DateMode = dmComboBox
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = #23435#20307
          Font.Style = []
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
          Kind = dtkDate
          ParseInput = False
          ParentFont = False
          TabOrder = 0
        end
        object txtSession: TEdit
          Left = 265
          Top = 26
          Width = 64
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = #23435#20307
          Font.Style = []
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
          ParentFont = False
          TabOrder = 1
          Text = '1'
        end
        object UpDown1: TUpDown
          Left = 329
          Top = 26
          Width = 16
          Height = 21
          Associate = txtSession
          Min = 1
          Max = 4
          Position = 1
          TabOrder = 2
          Wrap = False
        end
      end
    end
  end
  object BtnOk: TBitBtn
    Left = 146
    Top = 138
    Width = 100
    Height = 30
    Caption = ' '#20445#23384
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = BtnOkClick
    Kind = bkYes
  end
  object BtnCancel: TBitBtn
    Left = 266
    Top = 138
    Width = 100
    Height = 30
    Caption = ' '#36864#20986
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Kind = bkNo
  end
end
