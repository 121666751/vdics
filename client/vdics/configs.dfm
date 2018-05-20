object FormConfig: TFormConfig
  Left = 349
  Top = 194
  Width = 496
  Height = 392
  BorderIcons = [biSystemMenu]
  Caption = #37197#32622#21442#25968
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
    Width = 488
    Height = 338
    Align = alClient
    BevelInner = bvLowered
    BevelOuter = bvNone
    TabOrder = 0
    object Panel2: TPanel
      Left = 1
      Top = 1
      Width = 278
      Height = 336
      Align = alLeft
      TabOrder = 0
      object GroupBox1: TGroupBox
        Left = 8
        Top = 10
        Width = 258
        Height = 135
        Caption = #27969#31243
        TabOrder = 0
        object Label1: TLabel
          Left = 8
          Top = 23
          Width = 87
          Height = 13
          Caption = #20998#21457#26381#21153#22320#22336'     '
        end
        object Label2: TLabel
          Left = 30
          Top = 51
          Width = 63
          Height = 13
          Caption = #26381#21153#31471#21475'     '
        end
        object Label9: TLabel
          Left = 7
          Top = 79
          Width = 93
          Height = 13
          Caption = #21333#31080#20132#26131#20195#30721'       '
        end
        object Label7: TLabel
          Left = 7
          Top = 109
          Width = 93
          Height = 13
          Caption = #20449#23553#20132#26131#20195#30721'       '
        end
        object txtFlowip: TEdit
          Left = 95
          Top = 21
          Width = 142
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 20
          TabOrder = 0
        end
        object txtFlowport: TEdit
          Left = 95
          Top = 47
          Width = 96
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 8
          TabOrder = 1
        end
        object txtMsgType: TEdit
          Left = 94
          Top = 75
          Width = 96
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 6
          TabOrder = 2
        end
        object txtMsgTypeEx: TEdit
          Left = 94
          Top = 105
          Width = 96
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 6
          TabOrder = 3
        end
      end
      object GroupBox2: TGroupBox
        Left = 6
        Top = 152
        Width = 260
        Height = 80
        Caption = 'ECMClientProx'
        TabOrder = 1
        object Label3: TLabel
          Left = 30
          Top = 25
          Width = 63
          Height = 13
          Caption = #26381#21153#22320#22336'     '
        end
        object Label4: TLabel
          Left = 30
          Top = 57
          Width = 63
          Height = 13
          Caption = #26381#21153#31471#21475'     '
        end
        object txtEcmip: TEdit
          Left = 97
          Top = 22
          Width = 142
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 20
          TabOrder = 0
        end
        object txtEcmport: TEdit
          Left = 97
          Top = 52
          Width = 96
          Height = 21
          ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          MaxLength = 8
          TabOrder = 1
        end
      end
    end
    object Panel3: TPanel
      Left = 279
      Top = 1
      Width = 208
      Height = 336
      Align = alClient
      BevelInner = bvLowered
      Caption = 'Panel3'
      TabOrder = 1
      object Panel4: TPanel
        Left = 2
        Top = 2
        Width = 204
        Height = 279
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Panel4'
        TabOrder = 0
        object GroupBox4: TGroupBox
          Left = 7
          Top = 2
          Width = 186
          Height = 274
          Caption = #20998#34892
          TabOrder = 0
          object Label8: TLabel
            Left = 9
            Top = 25
            Width = 66
            Height = 13
            Caption = #20998#34892#21495#30721'      '
          end
          object Label10: TLabel
            Left = 13
            Top = 61
            Width = 54
            Height = 13
            Caption = '   '#21306#22495#21495'   '
          end
          object Label11: TLabel
            Left = 9
            Top = 90
            Width = 63
            Height = 13
            Caption = #20511#26041#36864#31080'     '
          end
          object Label12: TLabel
            Left = 11
            Top = 125
            Width = 69
            Height = 13
            Caption = #22788#29702#27169#24335'       '
          end
          object cbPre: TCheckBox
            Left = 12
            Top = 159
            Width = 97
            Height = 17
            Caption = #36827#34892#39044#25187
            TabOrder = 0
          end
          object cbSign: TCheckBox
            Left = 12
            Top = 188
            Width = 130
            Height = 17
            Caption = #36827#34892#36864#31080#30331#35760
            TabOrder = 1
          end
          object cbAmount: TCheckBox
            Left = 12
            Top = 213
            Width = 130
            Height = 25
            Caption = #19978#36865#37329#39069#20449#24687'   '
            TabOrder = 2
          end
          object txtBrno: TEdit
            Left = 75
            Top = 20
            Width = 98
            Height = 21
            ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
            MaxLength = 11
            TabOrder = 3
          end
          object txtAccno: TEdit
            Left = 75
            Top = 56
            Width = 98
            Height = 21
            ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
            MaxLength = 3
            TabOrder = 4
          end
          object txtDcode: TEdit
            Left = 75
            Top = 84
            Width = 98
            Height = 21
            ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
            MaxLength = 4
            TabOrder = 5
          end
          object cbMode: TComboBox
            Left = 75
            Top = 120
            Width = 102
            Height = 21
            Style = csDropDownList
            ItemHeight = 13
            ItemIndex = 0
            TabOrder = 6
            Text = #33258#34892#25195#25551
            Items.Strings = (
              #33258#34892#25195#25551
              #20154#34892#20195#25195
              #28165#20998#27169#24335)
          end
          object cbClear: TCheckBox
            Left = 13
            Top = 239
            Width = 130
            Height = 25
            Caption = #26816#26597#28165#31639#20449#24687' '
            TabOrder = 7
          end
        end
      end
      object Panel5: TPanel
        Left = 2
        Top = 281
        Width = 204
        Height = 53
        Align = alClient
        TabOrder = 1
        object btnSave: TButton
          Left = 18
          Top = 14
          Width = 75
          Height = 25
          Caption = #20445'  '#23384
          TabOrder = 0
          OnClick = btnSaveClick
        end
        object btnExit: TButton
          Left = 109
          Top = 14
          Width = 75
          Height = 25
          Caption = #36864'  '#20986
          TabOrder = 1
          OnClick = btnExitClick
        end
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 392
    Top = 47
    object N1: TMenuItem
      Caption = #25991#20214
      object numExit: TMenuItem
        Caption = #36864#20986
        OnClick = numExitClick
      end
    end
    object N8: TMenuItem
      Caption = #24110#21161#20449#24687
      object numAbout: TMenuItem
        Caption = #20851#20110
      end
    end
  end
end
