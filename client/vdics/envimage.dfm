object FormEnvImage: TFormEnvImage
  Left = 422
  Top = 207
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #31080#25454#22270#20687
  ClientHeight = 447
  ClientWidth = 673
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 673
    Height = 447
    Align = alClient
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 671
      Height = 352
      Align = alTop
      TabOrder = 0
      object image: TImage
        Left = 1
        Top = 1
        Width = 669
        Height = 350
        Align = alClient
        OnMouseUp = imageMouseUp
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 353
      Width = 671
      Height = 93
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 6
        Top = 3
        Width = 660
        Height = 88
        Caption = #22788#29702
        TabOrder = 0
        object Label3: TLabel
          Left = 8
          Top = 25
          Width = 39
          Height = 13
          Caption = #37329#39069'     '
        end
        object Label1: TLabel
          Left = 300
          Top = 25
          Width = 63
          Height = 13
          Caption = #29256#38754#31867#22411'     '
        end
        object Label2: TLabel
          Left = 165
          Top = 25
          Width = 66
          Height = 13
          Caption = #20973#35777#31867#22411'      '
        end
        object btnSave: TButton
          Left = 420
          Top = 50
          Width = 83
          Height = 25
          Caption = #20445'  '#23384
          ModalResult = 1
          TabOrder = 4
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 548
          Top = 50
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          ModalResult = 2
          TabOrder = 5
        end
        object txtAmount: TEdit
          Left = 45
          Top = 21
          Width = 100
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 16
          TabOrder = 0
          OnChange = txtAmountChange
          OnEnter = txtAmountEnter
          OnKeyPress = txtAmountKeyPress
        end
        object txtType: TComboBox
          Left = 376
          Top = 21
          Width = 81
          Height = 21
          Style = csDropDownList
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = #20449#23553#20973#35777
          Items.Strings = (
            #20449#23553#20973#35777
            #20195#25910#20973#35777
            #20195#20184#20973#35777
            #20854#23427#20973#35777)
        end
        object txtVchType: TEdit
          Left = 226
          Top = 21
          Width = 49
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 1
        end
        object chkMode: TCheckBox
          Left = 484
          Top = 21
          Width = 97
          Height = 17
          Caption = #26032#29256#38754
          TabOrder = 3
        end
      end
    end
  end
end
