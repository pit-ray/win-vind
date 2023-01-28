$(function() {
    $('html').toggleClass('no-js js');

    // Update sidebar highlighting based on Scrollspy
    $(window).on('activate.bs.scrollspy', function () {
        const spyTarget = $('[data-spy="scroll"]').data('target');
        const $activeSpy = $(spyTarget).find('.nav-link.active');
        const $tree = $activeSpy.parentsUntil('.bs-docs-sidenav', 'li');

        $tree.find('> a').addClass('active');
    });

    // Toggleable mobile table of contents button
    $('.toggle-toc').on('click', function () {
        const $this = $(this);
        const $toc = $("#mobileTOC");

        $toc.toggle();
        $this.attr('aria-expanded', $toc.is(':visible'));

        const $btn = $this.find('[data-role="toggle"]');

        if ($btn.text() === 'Hide') {
            $btn.text('Show');
        } else {
            $btn.text('Hide');
        }
    });

    // Make the triangular pattern in the header
    if (uiColors) {
        const $masthead = $('.site-masthead');

        if ($masthead.length) {
            const t = new Trianglify({
                cellsize: 90,
                noiseIntensity: 0,
                x_gradient: [
                    uiColors[0],
                    uiColors[1],
                ],
            });
            const pattern = t.generate(window.screen.width | $masthead.outerWidth(), $masthead.outerHeight() * 1.2);

            const style = $('<style>.site-masthead { background-image: ' + pattern.dataUrl + '; }</style>');
            $('html > head').append(style);
        }
    }
});
