object distFormImage: TdistFormImage
  Left = 192
  Top = 109
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #31080#25454#22270#20687
  ClientHeight = 442
  ClientWidth = 769
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
    Width = 769
    Height = 442
    Align = alClient
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 767
      Height = 336
      Align = alTop
      TabOrder = 0
      object image: TImage
        Left = 1
        Top = 1
        Width = 765
        Height = 334
        Align = alClient
        OnMouseUp = imageMouseUp
      end
    end
    object Panel3: TPanel
      Left = 1
      Top = 337
      Width = 767
      Height = 104
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 8
        Top = 8
        Width = 753
        Height = 89
        Caption = #22788#29702
        TabOrder = 0
        object Label1: TLabel
          Left = 198
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
          Left = 459
          Top = 28
          Width = 39
          Height = 13
          Caption = #37329#39069'     '
        end
        object Label4: TLabel
          Left = 328
          Top = 28
          Width = 63
          Height = 13
          Caption = #20973#35777#31867#22411'     '
        end
        object Label5: TLabel
          Left = 632
          Top = 26
          Width = 33
          Height = 13
          Caption = #29256#24335'   '
        end
        object btnSave: TButton
          Left = 555
          Top = 55
          Width = 83
          Height = 25
          Caption = #20445'  '#23384
          ModalResult = 1
          TabOrder = 4
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 651
          Top = 55
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          ModalResult = 2
          TabOrder = 5
        end
        object txtVchno: TEdit
          Left = 246
          Top = 24
          Width = 75
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 8
          TabOrder = 1
        end
        object txtAccno: TEdit
          Left = 37
          Top = 24
          Width = 153
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 21
          TabOrder = 0
        end
        object txtAmount: TEdit
          Left = 495
          Top = 24
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 12
          TabOrder = 3
          OnChange = txtAmountChange
          OnEnter = txtAmountEnter
          OnKeyPress = txtAmountKeyPress
        end
        object txtVchtype: TEdit
          Left = 393
          Top = 24
          Width = 49
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 4
          TabOrder = 2
        end
        object cbType: TComboBox
          Left = 665
          Top = 22
          Width = 64
          Height = 22
          Style = csOwnerDrawFixed
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 6
          Text = #31080'  '#25454
          Items.Strings = (
            #31080'  '#25454
            #20449'  '#23553)
        end
      end
    end
  end
end
