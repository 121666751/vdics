object FormNets: TFormNets
  Left = 231
  Top = 0
  Width = 609
  Height = 481
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = #20132#25442#32593#28857#28165#21333
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 593
    Height = 423
    Align = alClient
    BevelOuter = bvLowered
    Caption = 'Panel1'
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 591
      Height = 40
      Align = alTop
      Caption = #20132#25442#32593#28857
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object Panel3: TPanel
      Left = 1
      Top = 41
      Width = 591
      Height = 381
      Align = alClient
      BevelOuter = bvNone
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 417
        Height = 381
        Align = alLeft
        Caption = 'Panel4'
        TabOrder = 0
        object ListView: TListView
          Left = 1
          Top = 1
          Width = 415
          Height = 379
          Align = alClient
          BorderStyle = bsNone
          Color = clMoneyGreen
          Columns = <
            item
              Caption = #20132#25442#21495
              Width = 90
            end
            item
              Caption = #24402#23646#32593#28857
              Width = 90
            end
            item
              Caption = #20132#25442#21306#22495
              Width = 80
            end
            item
              Caption = #26159#21542#21305#37197
              Width = 80
            end
            item
              Caption = #26159#21542#26657#39564
              Width = 70
            end>
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ReadOnly = True
          RowSelect = True
          ParentFont = False
          PopupMenu = ErrResPopMenu
          TabOrder = 0
          ViewStyle = vsReport
          OnClick = ListViewClick
        end
      end
      object Panel5: TPanel
        Left = 417
        Top = 0
        Width = 174
        Height = 381
        Align = alClient
        BevelOuter = bvLowered
        TabOrder = 1
        object Label1: TLabel
          Left = 22
          Top = 11
          Width = 51
          Height = 13
          Caption = #20132#25442#21495'     '
        end
        object Label2: TLabel
          Left = 22
          Top = 95
          Width = 60
          Height = 13
          Caption = #24402#23646#32593#28857'    '
          Visible = False
        end
        object Label3: TLabel
          Left = 20
          Top = 142
          Width = 60
          Height = 13
          Caption = #20132#25442#21306#22495'    '
        end
        object txtNetno: TEdit
          Left = 22
          Top = 35
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 0
        end
        object BtnAdd: TButton
          Left = 45
          Top = 256
          Width = 75
          Height = 25
          Caption = #22686#21152
          TabOrder = 1
          OnClick = BtnAddClick
        end
        object Button3: TButton
          Left = 45
          Top = 359
          Width = 75
          Height = 25
          Caption = #36864#20986
          TabOrder = 2
          OnClick = Button3Click
        end
        object chbCancel: TCheckBox
          Left = 22
          Top = 65
          Width = 129
          Height = 25
          Caption = #20869#37096#25764#38144'   '
          TabOrder = 3
          OnClick = chbCancelClick
        end
        object txtNet2: TEdit
          Left = 22
          Top = 116
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 4
          Visible = False
        end
        object btnDelete: TButton
          Left = 45
          Top = 291
          Width = 75
          Height = 25
          Caption = #21047#26032
          TabOrder = 5
          OnClick = btnDeleteClick
        end
        object txtArea: TEdit
          Left = 19
          Top = 166
          Width = 121
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 6
        end
        object cbMerge: TCheckBox
          Left = 18
          Top = 201
          Width = 159
          Height = 17
          Caption = #26159#21542#19982#20154#34892#25968#25454#21305#37197'       '
          Checked = True
          State = cbChecked
          TabOrder = 7
        end
        object cbCheck: TCheckBox
          Left = 16
          Top = 228
          Width = 129
          Height = 17
          Caption = #26159#21542#29256#38754#26657#39564
          Checked = True
          State = cbChecked
          TabOrder = 8
        end
        object btnModif: TButton
          Left = 45
          Top = 324
          Width = 75
          Height = 25
          Caption = #20462#25913
          TabOrder = 9
          OnClick = btnModifClick
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 328
    Top = 80
    object N1: TMenuItem
      Caption = #25991#20214
      object N3: TMenuItem
        Caption = #36864#20986'[&E]'
        OnClick = N3Click
      end
    end
    object N2: TMenuItem
      Caption = #24110#21161#20449#24687
    end
  end
  object ErrResPopMenu: TPopupMenu
    Left = 114
    Top = 162
    object MeForceLook: TMenuItem
      Caption = #21024#38500'[&D] ...'
      OnClick = MeForceLookClick
    end
  end
end
