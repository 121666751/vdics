object FormWsImage: TFormWsImage
  Left = 278
  Top = 195
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
          Left = 316
          Top = 25
          Width = 63
          Height = 13
          Caption = #29256#38754#31867#22411'     '
        end
        object Label2: TLabel
          Left = 163
          Top = 26
          Width = 57
          Height = 13
          Caption = #20973#35777#31867#22411'   '
        end
        object btnSave: TButton
          Left = 404
          Top = 50
          Width = 83
          Height = 25
          Caption = #20445'  '#23384
          ModalResult = 1
          TabOrder = 3
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 516
          Top = 50
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          ModalResult = 2
          TabOrder = 4
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
          Left = 392
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
            #31532#19968#32852
            #31532#20108#32852)
        end
        object txtVchtype: TEdit
          Left = 224
          Top = 20
          Width = 65
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 1
        end
      end
    end
  end
end
