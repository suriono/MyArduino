Public Class MyOutlook

    Dim outlook_app As Microsoft.Office.Interop.Outlook.Application
    Dim outlook_calendar As Microsoft.Office.Interop.Outlook.MAPIFolder
    Dim outlook_session As Microsoft.Office.Interop.Outlook.NameSpace
    Dim outlook_items As Microsoft.Office.Interop.Outlook.Items
    Dim oOccur As Microsoft.Office.Interop.Outlook.AppointmentItem

    Public Function get_Current_Appointment()

        'Dim oItemsInDateRange As Microsoft.Office.Interop.Outlook.Items

        Dim myStart, myEnd As Date
        Dim rangeStr As String

        get_Current_Appointment = ""

        If Hour(Now) > 18 Then  ' after 6pm, no need to check calendar
            Exit Function
        End If


        outlook_app = CreateObject("Outlook.Application")
        outlook_session = outlook_app.Session
        outlook_items = outlook_session.GetDefaultFolder(Microsoft.Office.Interop.Outlook.OlDefaultFolders.olFolderCalendar).Items

        outlook_items.IncludeRecurrences = True
        outlook_items.Sort("[Start]")        'sorting the start time

        myStart = DateAdd("d", -60, Now)  'checking from 24 hours ago
        myEnd = DateAdd("d", 60, Now) ' from myStart to the next 24 hours

        rangeStr = "[Start]>='" + myStart.ToString("MM/dd/yyyy hh:mm tt") + "' AND [End]<='" + myEnd.ToString("MM/dd/yyyy hh:mm tt") + "'"



        'oItemsInDateRange = outlook_items.Restrict(rangeStr) 'restrict from 00:00 to 19:00 today

        For Each oOccur In outlook_items.Restrict(rangeStr)
            Application.DoEvents()

            If (DateTime.Compare(oOccur.Start, Now) < 0 And DateTime.Compare(oOccur.End, Now) > 0) Then

                If (oOccur.BusyStatus = Microsoft.Office.Interop.Outlook.OlBusyStatus.olOutOfOffice) Then
                    get_Current_Appointment = oOccur.Subject
                    Exit Function
                ElseIf (oOccur.BusyStatus = Microsoft.Office.Interop.Outlook.OlBusyStatus.olBusy) Then
                    If (oOccur.Location IsNot Nothing) Then
                        get_Current_Appointment = oOccur.Location
                        Exit Function
                    Else
                        get_Current_Appointment = oOccur.Subject
                    End If
                End If
            End If
        Next
    End Function

End Class
