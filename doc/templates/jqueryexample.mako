<!DOCTYPE html>
<html>
<head>
<title>Page Title</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.css" />
<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
<script src="http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.js"></script>
</head>
<body>
  <div data-url="demo-page" data-role="page" id="demo-page" data-theme="d">
    <div data-role="header" data-theme="c">
        <h1>${ci["CompanyName"]}</h1>
        <a href="#left-panel" data-icon="bars" data-iconpos="notext" data-shadow="false" data-iconshadow="false">Menu</a>
    </div><!-- /header -->
    <div data-role="content">
        <div class="article">
            <p><img src="../../_assets/img/bike.jpg" alt="Fixed Gear bike"></p>
            <h2>${ci["SystemTitle"]}</h2>
            <p>${ci["SystemSummary"]}</p>
            <p><small>Copyright 2013 ${ci["CompanyName"]}</small></p>
            <p><a href="#right-panel" data-role="button" data-theme="b" data-inline="true" data-mini="true" data-shadow="false">Share</a>
               <a href="#right-panel" data-role="button" data-theme="b" data-inline="true" data-mini="true" data-shadow="false">Login</a></p>
        </div><!-- /article -->
    </div><!-- /content -->
    <div data-role="panel" id="left-panel" data-theme="c">
        <ul data-role="listview" data-theme="d">
            <li data-icon="delete"><a href="#" data-rel="close">Close</a></li>
            <li data-role="list-divider">Menu</li>
        </ul>
        <div data-role="collapsible" data-inset="false" data-iconpos="right" data-theme="d" data-content-theme="d">
          <h3>Categories</h3>
                
   % for majorgroup in di.keys():
          <div data-role="collapsible-set" data-inset="false" data-iconpos="right" data-theme="b" data-content-theme="d">
          <div data-role="collapsible">
          <h3>${majorgroup}</h3>
            <ul data-role="listview">
     % for minorgroup in di[majorgroup].keys():
              <li><a href="#">${minorgroup}</a></li>
     % endfor
            </ul>
          </div><!-- /collapsible -->
          </div><!-- /collapsible -->
   % endfor
       </div><!-- /collapsible -->
                
    </div><!-- /panel -->
    <div data-role="panel" id="right-panel" data-display="overlay" data-position="right" data-theme="c">
        <ul data-role="listview" data-theme="d" data-icon="false">
            <li data-icon="delete"><a href="#" data-rel="close">Close</a></li>
            <li data-role="list-divider">Share this page</li>
            <li><a href="#">Twitter</a></li>
            <li><a href="#">Facebook</a></li>
            <li><a href="#">Google +</a></li>
            <li><a href="#">Mail</a></li>
        </ul>
    </div><!-- /panel -->
</div>
</body>
</html>
