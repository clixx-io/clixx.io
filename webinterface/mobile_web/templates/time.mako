<%!
    ## python module-level code
    import datetime
%>
<%
    ## python code block
    def getDate():
        return datetime.datetime.now()
%>
<html><head></head><body>
<h1>It is now ${getDate()}</h1>
</body></html>
