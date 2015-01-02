{
  'targets': [
    {
      'target_name': 'server',
      'type': 'executable',
      'sources': [
        'src/pal.h',
        'src/server.c',
        'src/server.h',
        '<(INTERMEDIATE_DIR)/packaged-html-files.c',
      ],
      'dependencies': [
        'mongoose',
      ],
      'actions': [
        {
          'action_name': 'package html files',
          'inputs': [
            'files-to-c-arrays.py',
            'html/compatibility.js',
            'html/index.html',
            'html/keep-server-alive.js',
          ],
          'outputs': [
            '<(INTERMEDIATE_DIR)/packaged-html-files.c',
          ],
          'action': ['python', 'files-to-c-arrays.py', '<@(_outputs)', '<@(_inputs)'],
          'msvs_cygwin_shell': 0,
        },
      ],
      'conditions': [
        ['OS=="linux"', {
          'sources': [
            'src/linux/pal.c',
            'src/linux/main.c',
          ],
        }],
        ['OS=="win"', {
          'sources': [
            'src/win/getopt.c',
            'src/win/main.cpp',
            'src/win/pal.cpp',
            'src/win/stdafx.h',
          ],
        }],
        ['OS=="mac"', {
          'sources': [
            'src/mac/main.m',
            'src/mac/pal.m',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
              '$(SDKROOT)/System/Library/Frameworks/CoreVideo.framework',
              '$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
            ],
          },
        }],
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'CompileAs': 2, # Compile C as C++, since msvs doesn't support C99
        },
        'VCLinkerTool': {
          'AdditionalDependencies': [
            'winmm.lib',
            'setupapi.lib',
          ],
        },
      },
    },
    {
      'target_name': 'mongoose',
      'type': 'static_library',
      'sources': [
        'third_party/mongoose/mongoose.c',
        'third_party/mongoose/mongoose.h',
      ],
    },
  ],

  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      'Debug': {
        'defines': [ 'DEBUG', '_DEBUG' ],
        'cflags': [ '-g', '-O0' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 1, # static debug
            'Optimization': 0, # optimizeDisabled (/Od)
          },
        },
        'xcode_settings': {'GCC_OPTIMIZATION_LEVEL': 0 } # -O0 No optimization
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'cflags': [ '-Os' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'RuntimeLibrary': 0, # static release
            'Optimization': 3, # full optimization (/Ox)
          },
        },
        'xcode_settings': { 'GCC_OPTIMIZATION_LEVEL': 's' }, # -Os optimize for size+speed
      }
    },
    'conditions': [
      ['OS=="linux"', {
        'ldflags': [ '-pthread' ],
        'link_settings': {
          'libraries' : [
          '-ldl',
          '-lX11',
          '-lXtst',
          '-lGL',
          '-ludev',
          '-lXinerama',
          ],
        },
      }],
      ['OS=="win"', {
        'defines': [ '_WINDOWS', 'WIN32' ],
      }],
      ['OS=="mac"', {
        'xcode_settings': {
          'ARCHS': '$(ARCHS_STANDARD_64_BIT)',
        },
      }],
    ],
    'msvs_settings': {
      'VCLinkerTool': {
        'GenerateDebugInformation': 'true',
        'SubSystem': 2, # Windows
      },
    },
    'xcode_settings': {
      # This prevents a silly warning in XCode's UI ("Update project to recommended settings"). Unfortunately there's no way to prevent the warning on 'action' targets; oh well.
      'COMBINE_HIDPI_IMAGES': 'YES',
    },
  },

  'conditions': [
    ['OS=="mac"',
      # The XCode project generator automatically adds a bogus "All" target with bad xcode_settings unless we define one here.
      {
        'targets': [
          {
            'target_name': 'All',
            'type': 'static_library',
          },
        ],
      },
    ],
  ],
}
