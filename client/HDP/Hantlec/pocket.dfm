object FmPocket: TFmPocket
  Left = 289
  Top = 177
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #28165#20998#21475#34955#35774#32622
  ClientHeight = 382
  ClientWidth = 710
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 710
    Height = 382
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 496
      Height = 380
      Align = alLeft
      Caption = 'Panel2'
      TabOrder = 0
      object Panel4: TPanel
        Left = 1
        Top = 1
        Width = 494
        Height = 41
        Align = alTop
        BevelOuter = bvNone
        Caption = #28165#20998#21475#34955#35774#32622
        Color = clMoneyGreen
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object lstPocket: TListView
        Left = 1
        Top = 42
        Width = 494
        Height = 337
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = #27969#27700#21495
            Width = 90
          end
          item
            Caption = #20973#35777#31867#22411
            Width = 90
          end
          item
            Caption = #21475#34955#32534#21495
            Width = 90
          end
          item
            Caption = #21475#34955#25551#36848
            Width = 90
          end
          item
            Caption = #28165#20998#27969#31243
            Width = 130
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        RowSelect = True
        ParentFont = False
        PopupMenu = ErrResPopMenu
        TabOrder = 1
        ViewStyle = vsReport
      end
    end
    object Panel3: TPanel
      Left = 497
      Top = 1
      Width = 212
      Height = 380
      Align = alClient
      TabOrder = 1
      object GroupBox1: TGroupBox
        Left = 8
        Top = 76
        Width = 193
        Height = 186
        Caption = #25968#25454
        TabOrder = 0
        object Label1: TLabel
          Left = 7
          Top = 29
          Width = 60
          Height = 13
          Caption = #20973#35777#31867#22411'    '
        end
        object Label2: TLabel
          Left = 7
          Top = 108
          Width = 54
          Height = 13
          Caption = #21475#34955#25551#36848'  '
        end
        object Label4: TLabel
          Left = 7
          Top = 148
          Width = 54
          Height = 13
          Caption = #28165#20998#27969#31243'  '
        end
        object Label6: TLabel
          Left = 7
          Top = 68
          Width = 60
          Height = 13
          Caption = #21475#34955#32534#21495'    '
        end
        object txtPockdesc: TEdit
          Left = 74
          Top = 105
          Width = 103
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 40
          TabOrder = 0
        end
        object txtName: TEdit
          Left = 98
          Top = 189
          Width = 110
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 20
          TabOrder = 1
        end
        object txtVchtype: TComboBox
          Left = 73
          Top = 25
          Width = 104
          Height = 19
          Style = csOwnerDrawFixed
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 2
          Text = #19968#27425#28165#20998
          Items.Strings = (
            #19968#27425#28165#20998
            #20108#27425#28165#20998
            #19977#27425#28165#20998)
        end
        object txtPockno: TEdit
          Left = 73
          Top = 64
          Width = 88
          Height = 21
          ReadOnly = True
          TabOrder = 3
          Text = '1'
        end
        object ud_pockno: TUpDown
          Left = 161
          Top = 64
          Width = 16
          Height = 21
          Associate = txtPockno
          Min = 1
          Max = 10
          Position = 1
          TabOrder = 4
          Wrap = False
        end
        object txtDistno: TEdit
          Left = 73
          Top = 144
          Width = 88
          Height = 21
          ReadOnly = True
          TabOrder = 5
          Text = '1'
        end
        object UpDown1: TUpDown
          Left = 161
          Top = 144
          Width = 16
          Height = 21
          Associate = txtDistno
          Min = 1
          Max = 3
          Position = 1
          TabOrder = 6
          Wrap = False
        end
      end
      object btnAdd: TButton
        Left = 67
        Top = 293
        Width = 75
        Height = 25
        Caption = #22686'  '#21152
        TabOrder = 1
        OnClick = btnAddClick
      end
      object btnExit: TButton
        Left = 68
        Top = 333
        Width = 75
        Height = 25
        Caption = #36864'  '#20986
        TabOrder = 2
        OnClick = btnExitClick
      end
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
