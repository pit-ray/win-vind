---
layout: notoc
title: Insights - win-vind
nav: Insights
show_in_menu: false
disable_anchors: true
---  

## Download Count  
<div>Total: <span id="total">0</span></div>
<canvas id="dl_count"></canvas>  

<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js"></script>  

<br>    

<script>  
var request = new XMLHttpRequest();
request.open('GET', 'https://api.github.com/repos/pit-ray/win-vind/releases');  

request.onreadystatechange = function() {
  var names = [];
  var counts = [];
  var total = 0;

  if(request.readyState == 4) {
    if (request.status == 200) {
      var data = JSON.parse(request.responseText);
      for(var item of data) {
        var sp = item.name.split('.');
        var version = sp[0] + '.' + sp[1] + '.x';

        var idx = names.indexOf(version);
        if(idx == -1) {
          names.push(version);
        }

        var cnt = 0 ;
        for(var a of item.assets) {
          cnt += a.download_count;
        }

        if(idx == -1) {
          counts.push(cnt);
        }
        else {
          counts[idx] += cnt;
        }
        total += cnt;
      }
    }
  }
  
  const total_ctx = document.getElementById('total');
  total_ctx.innerText = total;

  const chart_ctx = document.getElementById('dl_count');
  const chart = new Chart(chart_ctx, {
    type: 'line',
    data: {
      labels: names.reverse(),
      datasets: [
        {
          label: 'count',
          data: counts.reverse()
        }
      ]
    },
    options: {
      legend: {
        display: false
      }
    }
  });
};
request.send();
</script>
