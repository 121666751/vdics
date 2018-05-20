object FormImage: TFormImage
  Left = 279
  Top = 287
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #31080#25454#22270#20687
  ClientHeight = 442
  ClientWidth = 693
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
    Width = 693
    Height = 442
    Align = alClient
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 691
      Height = 336
      Align = alTop
      TabOrder = 0
      object image: TImage
        Left = 1
        Top = 1
        Width = 689
        Height = 334
        Align = alClient
        OnMouseUp = imageMouseUp
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 337
      Width = 691
      Height = 104
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 8
        Top = 8
        Width = 673
        Height = 89
        Caption = #22788#29702
        TabOrder = 0
        object Label1: TLabel
          Left = 207
          Top = 28
          Width = 51
          Height = 13
          Caption = #20973#35777#21495'     '
        end
        object Label2: TLabel
          Left = 8
          Top = 28
          Width = 39
          Height = 13
          Caption = #24080#21495'     '
        end
        object Label3: TLabel
          Left = 479
          Top = 28
          Width = 39
          Height = 13
          Caption = #37329#39069'     '
        end
        object Label4: TLabel
          Left = 357
          Top = 23
          Width = 54
          Height = 13
          Caption = #20973#35777#31867#22411'  '
        end
        object Label5: TLabel
          Left = 365
          Top = 38
          Width = 30
          Height = 13
          Caption = '('#20195#30721')'
        end
        object btnSave: TButton
          Left = 465
          Top = 54
          Width = 83
          Height = 25
          Caption = #20445'  '#23384
          ModalResult = 1
          TabOrder = 5
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 561
          Top = 54
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          ModalResult = 2
          TabOrder = 6
        end
        object txtVchno: TEdit
          Left = 249
          Top = 24
          Width = 102
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 16
          TabOrder = 1
        end
        object txtAccno: TEdit
          Left = 48
          Top = 24
          Width = 153
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 21
          TabOrder = 0
        end
        object txtAmount: TEdit
          Left = 515
          Top = 24
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 16
          TabOrder = 3
          OnChange = txtAmountChange
          OnEnter = txtAmountEnter
          OnKeyPress = txtAmountKeyPress
        end
        object txtVchtype: TEdit
          Left = 413
          Top = 24
          Width = 49
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 2
        end
        object chkMode: TCheckBox
          Left = 9
          Top = 61
          Width = 97
          Height = 17
          Caption = #26032#29256#38754
          TabOrder = 4
        end
      end
    end
  end
end
