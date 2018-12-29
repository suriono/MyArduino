function draw() {
	var canvas = document.getElementById("canvas");
    	if (null==canvas || !canvas.getContext) return;
	var axes={}, ctx=canvas.getContext("2d");
	axes.x0 = .0 + 0*canvas.width;  // x0 pixels from left to x=0
	axes.y0 = 0 + 0.9*canvas.height; // y0 pixels from top to y=0
 	axes.doNegativeX = true;
 	showAxes(ctx,axes);
 	funGraph(ctx,axes,"rgb(11,153,11)",1); 
}
function showAxes(ctx,axes) {
 	var x0=axes.x0, w=ctx.canvas.width;
	var y0=axes.y0, h=ctx.canvas.height;
	var xmin = axes.doNegativeX ? 0 : x0;
	ctx.beginPath();
	ctx.strokeStyle = "rgb(128,128,128)"; 
 	ctx.moveTo(xmin,y0); ctx.lineTo(w,y0);  // X axis
 	ctx.moveTo(x0,0);    ctx.lineTo(x0,h);  // Y axis
 	ctx.stroke();
}
function funGraph (ctx,axes,color,thick) {
 	var xx, yy, x0=axes.x0, y0=axes.y0// , scale=axes.scale;
 	ctx.beginPath();
 	ctx.lineWidth = thick;
 	ctx.strokeStyle = color;
	
	//Arduino enter the data below:
        //var dist = new Array();
        //dist = distance();

	var dist_max = Math.max.apply(null,dist);

	var time_max = dist_interval*dist.length / 60; // conversion to hour

	var dx = canvas.width / dist.length;
 	for (var i=0;i<dist.length;i++) {
  	   xx = dx*i; 
	   yy = dist[i] * y0 / dist_max;
	   ctx.lineTo(x0+xx,y0-yy);
 	}
 	ctx.stroke();

	ctx.save();
	ctx.textAlign = 'left';
	ctx.fillText(dist_max + "cm",x0,10)
	ctx.translate(x0+xx, y0);
	ctx.rotate(3.14 * 1.5);
	ctx.fillText(time_max.toPrecision(3) + " Hour",0,0);
	ctx.restore();
}

