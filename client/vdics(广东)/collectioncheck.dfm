object FormWsCheck: TFormWsCheck
  Left = 135
  Top = 142
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #22996#25910#26680#25171
  ClientHeight = 412
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 412
    Align = alClient
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 216
      Height = 410
      Align = alLeft
      Caption = 'Panel2'
      TabOrder = 0
      object lstFile: TListView
        Left = 1
        Top = 1
        Width = 214
        Height = 408
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #24207#21495
            Width = 70
          end
          item
            Caption = #25991#20214#21517
            Width = 140
          end>
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnSelectItem = lstFileSelectItem
      end
    end
    object Panel3: TPanel
      Left = 217
      Top = 1
      Width = 574
      Height = 410
      Align = alClient
      Caption = 'Panel3'
      TabOrder = 1
      object LabelAmount: TLabel
        Left = 96
        Top = 304
        Width = 36
        Height = 13
        Caption = #37329#39069'    '
        Visible = False
      end
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 572
        Height = 280
        Align = alTop
        TabOrder = 0
        object image: TImage
          Left = 1
          Top = 1
          Width = 570
          Height = 278
          Align = alClient
          OnMouseUp = imageMouseUp
        end
      end
      object btnExit: TButton
        Left = 464
        Top = 360
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 1
        OnClick = btnExitClick
      end
      object btnSave: TButton
        Left = 248
        Top = 360
        Width = 75
        Height = 25
        Caption = #20445#23384
        TabOrder = 2
        OnClick = btnSaveClick
      end
      object btnCheck: TButton
        Left = 358
        Top = 360
        Width = 74
        Height = 25
        Caption = #26680#25171
        TabOrder = 3
        OnClick = btnCheckClick
      end
      object txtAmount: TEdit
        Left = 139
        Top = 300
        Width = 121
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 4
        Visible = False
        OnChange = txtAmountChange
        OnEnter = txtAmountEnter
        OnKeyPress = txtAmountKeyPress
      end
      object chkMain: TCheckBox
        Left = 16
        Top = 304
        Width = 73
        Height = 17
        Caption = #20027#20214
        TabOrder = 5
        OnClick = chkMainClick
      end
    end
  end
end
