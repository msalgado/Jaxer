cfg = {
        'jam_install':          '/opt/AptanaJaxer',
        'unzip_location':       '/opt',
        'start_servers':        '/opt/AptanaJaxer/scripts/start.sh',
        'stop_servers':         '/opt/AptanaJaxer/scripts/stop.sh',
        'httpd_pid':            '/opt/AptanaJaxer/Apache22/logs/httpd.pid',
        'results_file':         '/opt/AptanaJaxer/jaxer/aptana/diagnostics/results.html',
        'jam_package':          '/export/home/build/Jaxer/server/distro/Jaxer_package_withApache.zip',
        'jam_mount':            '',

        'apache-name':          'httpd',
        'apxs-name':		'LTFLAGS="--tag=CC" /usr/apache2/bin/apxs -c -S CC=/opt/solstudio12.2/bin/cc',
        'jaxermanager-name':    'jaxermanager',
        'browser':              'firefox',
        'browser-name':         'firefox',
        'kill':                 'killall',

        'host':                 'http://localhost:8081',
        'page':                 'aptana/diagnostics/testRunner.html',
        'query':                'runAll=true&writeTo=results.html',

        'makeDocs':             True,
        'frameworkRoot':        '/export/home/build/Jaxer/server/src/mozilla/ff-release/dist/bin/framework',
        'docgenRoot':           '/export/home/build/Jaxer/server/tools/ide_crossover/docgen/libs/',
        'docRoot':              '/export/home/build/Jaxer/server/jam/jaxer/aptana/doc/api/',

        'skipBuilds':           False
}

