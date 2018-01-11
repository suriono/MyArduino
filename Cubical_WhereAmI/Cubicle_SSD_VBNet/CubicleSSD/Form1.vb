Imports System.IO.Ports


Public Class Form1

    Dim Send_to_SSD As String
    Dim myOutlook As MyOutlook
    Dim Last_in_office As Date

    Private Sub Form1_Load() Handles Me.Load
        'Label_debug.Text = "Form1 loading"
        System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = False 'to remove the Cross-thread operation not valid error

    End Sub
    Private Sub Form1_Close() Handles Me.Closing
        SerialPort_SSD.Close()
    End Sub

    Private Sub Button_Start_Click(sender As Object, e As EventArgs) Handles Button_Start.Click
        SerialPort_SSD.PortName = ComboBox_COM_SSD.Text
        SerialPort_UltraSonic.PortName = ComboBox_COM_UltraSonic.Text
        SerialPort_SSD.Open()
        SerialPort_UltraSonic.Open()

        myOutlook = New MyOutlook()
        Last_in_office = Now()

        Label_status.Text = "Starting the communications ...."
    End Sub


    Private Sub Serial_Received_SSD(sender As Object, e As SerialDataReceivedEventArgs) Handles SerialPort_SSD.DataReceived
        Dim sp As SerialPort
        Dim readstr As String
        sp = sender
        readstr = sp.ReadLine().Trim()

        Label_received_display.Text = readstr

        If (readstr.Length() > 0 And readstr = Send_to_SSD) Then ' if sending succesful
            Label_status.Text = "Succesful:" + readstr + "      " + Now.ToString()

        ElseIf (readstr = "Request") Then

            Dim seconds_elapsed As Integer
            seconds_elapsed = Now.Subtract(Last_in_office).TotalSeconds

            'Label_debug.Text = "elapse: " + seconds_elapsed.ToString() + "," + Last_in_office.ToString

            If seconds_elapsed < 30 Then                    ' 30 seconds since last sensed by ultrasonic
                Send_to_SSD = "Welcome(1)to Uz's Cubicle(2)"
            ElseIf (myOutlook.get_Current_Appointment.ToString.Length() > 1) Then
                Label_status.Text = "Requesting Outlook Calendar ..... "
                Send_to_SSD = "Calendar:(1)" + myOutlook.get_Current_Appointment + "(2)"
            ElseIf Now.Hour() > 17 Then
                Send_to_SSD = Now.ToShortTimeString + "(1)I am gone(2)"
            ElseIf Now.Hour() < 7 Then
                Send_to_SSD = Now.ToShortTimeString + "(1)I am not here yet(2)"
            ElseIf seconds_elapsed > 36000 Then
                Send_to_SSD = Now.ToShortTimeString + "(1)I am not here yet(2)"
            Else
                Dim displaytime As String
                If seconds_elapsed < 60 Then
                    displaytime = seconds_elapsed.ToString + " seconds ago"
                Else
                    displaytime = Int(seconds_elapsed / 60).ToString + " minutes ago"
                End If
                    Send_to_SSD = "I was here(1)" + displaytime + "(2)"
                End If
                SerialPort_SSD.WriteLine(Send_to_SSD + "<")
            Else
                SerialPort_SSD.WriteLine(Send_to_SSD + "<") ' resend if fail
        End If
    End Sub

    Private Sub Serial_Received_UltraSonic(sender As Object, e As SerialDataReceivedEventArgs) Handles SerialPort_UltraSonic.DataReceived
        Dim sp As SerialPort
        Dim readstr As String
        sp = sender
        readstr = sp.ReadLine().Trim()

        Label_received_ultrasonic.Text = readstr

        If StrComp(readstr, "in_office") = 0 Then
            Last_in_office = Now()
            'Label_debug.Text = "true" + Last_in_office.ToString() + StrComp(readstr, "in_office").ToString
            'Else
            'Label_debug.Text = "false" + Last_in_office.ToString()
        End If
        'Label_debug.Text = Last_in_office.ToString()
        'Label_debug.Text = "Received from Ultrasonic: " + readstr
    End Sub

    Private Sub Button_debug_Click(sender As Object, e As EventArgs) Handles Button_debug.Click
        Send_to_SSD = "Again(1)hello(2)"
        SerialPort_SSD.WriteLine(Send_to_SSD + "<")
    End Sub


End Class
