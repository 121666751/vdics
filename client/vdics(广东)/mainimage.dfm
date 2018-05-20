object FormMainImage: TFormMainImage
  Left = 422
  Top = 207
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #20027#20214#22270#20687
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
        object Label2: TLabel
          Left = 23
          Top = 29
          Width = 66
          Height = 13
          Caption = #20973#35777#31867#22411'      '
        end
        object btnSave: TButton
          Left = 413
          Top = 48
          Width = 83
          Height = 25
          Caption = #20445'  '#23384
          ModalResult = 1
          TabOrder = 1
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 525
          Top = 48
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          ModalResult = 2
          TabOrder = 2
        end
        object txtVchtype: TEdit
          Left = 80
          Top = 25
          Width = 49
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 0
        end
        object chkMode: TCheckBox
          Left = 168
          Top = 26
          Width = 97
          Height = 17
          Caption = #26032#29256#38754
          TabOrder = 3
        end
      end
    end
  end
end
