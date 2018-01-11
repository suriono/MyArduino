<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.Label_received_ultrasonic = New System.Windows.Forms.Label()
        Me.Label_received_display = New System.Windows.Forms.Label()
        Me.ComboBox_COM_UltraSonic = New System.Windows.Forms.ComboBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Button_Start = New System.Windows.Forms.Button()
        Me.ComboBox_COM_SSD = New System.Windows.Forms.ComboBox()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.Button_debug = New System.Windows.Forms.Button()
        Me.Label_debug = New System.Windows.Forms.Label()
        Me.SerialPort_SSD = New System.IO.Ports.SerialPort(Me.components)
        Me.SerialPort_UltraSonic = New System.IO.Ports.SerialPort(Me.components)
        Me.Label_status = New System.Windows.Forms.Label()
        Me.GroupBox1.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.SuspendLayout()
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.Label_received_ultrasonic)
        Me.GroupBox1.Controls.Add(Me.Label_received_display)
        Me.GroupBox1.Controls.Add(Me.ComboBox_COM_UltraSonic)
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.Label1)
        Me.GroupBox1.Controls.Add(Me.Button_Start)
        Me.GroupBox1.Controls.Add(Me.ComboBox_COM_SSD)
        Me.GroupBox1.Location = New System.Drawing.Point(5, 1)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(424, 143)
        Me.GroupBox1.TabIndex = 0
        Me.GroupBox1.TabStop = False
        '
        'Label_received_ultrasonic
        '
        Me.Label_received_ultrasonic.AutoSize = True
        Me.Label_received_ultrasonic.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_received_ultrasonic.Location = New System.Drawing.Point(102, 88)
        Me.Label_received_ultrasonic.Name = "Label_received_ultrasonic"
        Me.Label_received_ultrasonic.Size = New System.Drawing.Size(180, 16)
        Me.Label_received_ultrasonic.TabIndex = 6
        Me.Label_received_ultrasonic.Text = "received data from ultrasonic"
        '
        'Label_received_display
        '
        Me.Label_received_display.AutoSize = True
        Me.Label_received_display.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_received_display.Location = New System.Drawing.Point(102, 33)
        Me.Label_received_display.Name = "Label_received_display"
        Me.Label_received_display.Size = New System.Drawing.Size(167, 16)
        Me.Label_received_display.TabIndex = 5
        Me.Label_received_display.Text = "received data from display"
        '
        'ComboBox_COM_UltraSonic
        '
        Me.ComboBox_COM_UltraSonic.FormattingEnabled = True
        Me.ComboBox_COM_UltraSonic.Items.AddRange(New Object() {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8"})
        Me.ComboBox_COM_UltraSonic.Location = New System.Drawing.Point(10, 85)
        Me.ComboBox_COM_UltraSonic.Name = "ComboBox_COM_UltraSonic"
        Me.ComboBox_COM_UltraSonic.Size = New System.Drawing.Size(65, 21)
        Me.ComboBox_COM_UltraSonic.TabIndex = 4
        Me.ComboBox_COM_UltraSonic.Text = "COM8"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(7, 69)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(59, 13)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "Ultra Sonic"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(7, 16)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(41, 13)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Display"
        '
        'Button_Start
        '
        Me.Button_Start.Location = New System.Drawing.Point(6, 112)
        Me.Button_Start.Name = "Button_Start"
        Me.Button_Start.Size = New System.Drawing.Size(105, 25)
        Me.Button_Start.TabIndex = 1
        Me.Button_Start.Text = "START"
        Me.Button_Start.UseVisualStyleBackColor = True
        '
        'ComboBox_COM_SSD
        '
        Me.ComboBox_COM_SSD.FormattingEnabled = True
        Me.ComboBox_COM_SSD.Items.AddRange(New Object() {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10"})
        Me.ComboBox_COM_SSD.Location = New System.Drawing.Point(9, 32)
        Me.ComboBox_COM_SSD.MaxDropDownItems = 10
        Me.ComboBox_COM_SSD.Name = "ComboBox_COM_SSD"
        Me.ComboBox_COM_SSD.Size = New System.Drawing.Size(66, 21)
        Me.ComboBox_COM_SSD.TabIndex = 0
        Me.ComboBox_COM_SSD.Text = "COM9"
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.Button_debug)
        Me.GroupBox2.Controls.Add(Me.Label_debug)
        Me.GroupBox2.Location = New System.Drawing.Point(12, 177)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(319, 64)
        Me.GroupBox2.TabIndex = 1
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Debug"
        '
        'Button_debug
        '
        Me.Button_debug.Location = New System.Drawing.Point(9, 35)
        Me.Button_debug.Name = "Button_debug"
        Me.Button_debug.Size = New System.Drawing.Size(75, 23)
        Me.Button_debug.TabIndex = 1
        Me.Button_debug.Text = "Debug"
        Me.Button_debug.UseVisualStyleBackColor = True
        '
        'Label_debug
        '
        Me.Label_debug.AutoSize = True
        Me.Label_debug.Location = New System.Drawing.Point(6, 16)
        Me.Label_debug.Name = "Label_debug"
        Me.Label_debug.Size = New System.Drawing.Size(37, 13)
        Me.Label_debug.TabIndex = 0
        Me.Label_debug.Text = "debug"
        '
        'SerialPort_SSD
        '
        Me.SerialPort_SSD.ReadBufferSize = 1024
        Me.SerialPort_SSD.WriteBufferSize = 1024
        '
        'SerialPort_UltraSonic
        '
        Me.SerialPort_UltraSonic.ReadBufferSize = 512
        Me.SerialPort_UltraSonic.WriteBufferSize = 512
        '
        'Label_status
        '
        Me.Label_status.AutoSize = True
        Me.Label_status.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label_status.ForeColor = System.Drawing.Color.Red
        Me.Label_status.Location = New System.Drawing.Point(5, 147)
        Me.Label_status.Name = "Label_status"
        Me.Label_status.Size = New System.Drawing.Size(48, 16)
        Me.Label_status.TabIndex = 2
        Me.Label_status.Text = "Status:"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(441, 186)
        Me.Controls.Add(Me.Label_status)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents Button_Start As Button
    Friend WithEvents ComboBox_COM_SSD As ComboBox
    Friend WithEvents GroupBox2 As GroupBox
    Friend WithEvents Label_debug As Label
    Friend WithEvents SerialPort_SSD As IO.Ports.SerialPort
    Friend WithEvents ComboBox_COM_UltraSonic As ComboBox
    Friend WithEvents Label2 As Label
    Friend WithEvents Label1 As Label
    Friend WithEvents SerialPort_UltraSonic As IO.Ports.SerialPort
    Friend WithEvents Button_debug As Button
    Friend WithEvents Label_status As Label
    Friend WithEvents Label_received_ultrasonic As Label
    Friend WithEvents Label_received_display As Label
End Class
