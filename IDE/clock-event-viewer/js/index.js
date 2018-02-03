    $(function() {

        $(".knob").knob({
            draw : function () {

                // "tron" case
                if(this.$.data('skin') == 'tron') {

                    var a = this.angle(this.cv)  // Angle
                            , sa = this.startAngle          // Previous start angle
                            , sat = this.startAngle         // Start angle
                            , ea                            // Previous end angle
                            , eat = sat + a                 // End angle
                            , r = true;

                    this.g.lineWidth = this.lineWidth;

                    this.o.cursor
                    && (sat = eat - 0.3)
                    && (eat = eat + 0.3);

                    if (this.o.displayPrevious) {
                        ea = this.startAngle + this.angle(this.value);
                        this.o.cursor
                        && (sa = ea - 0.3)
                        && (ea = ea + 0.3);
                        this.g.beginPath();
                        this.g.strokeStyle = this.previousColor;
                        this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sa, ea, false);
                        this.g.stroke();
                    }

                    this.g.beginPath();
                    this.g.strokeStyle = r ? this.o.fgColor : this.fgColor ;
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth, sat, eat, false);
                    this.g.stroke();

                    this.g.lineWidth = 2;
                    this.g.beginPath();
                    this.g.strokeStyle = this.o.fgColor;
                    this.g.arc(this.xy, this.xy, this.radius - this.lineWidth + 1 + this.lineWidth * 2 / 3, 0, 2 * Math.PI, false);
                    this.g.stroke();

                    return false;
                }
            }
        });
    });

    function tp (s) {
        s += "";
        if (s.length === 1)
            s = "0" + s;
        return s;
    }
    function clock() {
        var $s = $(".second");
        var $m = $(".minute");
        var $h = $(".hour");
        var d = new Date();
        var s = d.getSeconds();
        var ms = d.getMilliseconds();
        var m = d.getMinutes();
        var h = d.getHours();
        $s.val((s*1000)+ms).trigger("change");
        $m.val((m * 60)+s).trigger("change");
        $h.val((h*60)+m).trigger("change");

        $('span.hour').html(tp(h));
        $('span.minute').html(tp(m));
        $('span.second').html(tp(s));
        setTimeout("clock()", 30);
    }
    clock();