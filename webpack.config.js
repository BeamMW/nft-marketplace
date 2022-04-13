const path = require('path')
const html = require('html-webpack-plugin')
const copy = require('copy-webpack-plugin')
const extractCSS = require('mini-css-extract-plugin')
const {VueLoaderPlugin} = require('vue-loader')
const ESLint = require('eslint-webpack-plugin')
const SLint = require('stylelint-webpack-plugin')

const config = (DEV_MODE) => {return {
  entry: {
    main: './src/main.js',
  },
  devServer: {
    hot: true,
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp'
    }
  },
  output: {
    filename: '[name].bundle.js',
    path: path.resolve(__dirname, 'html'),
    clean: true
  },
  resolve: {
    alias: {
      'assets': path.resolve(__dirname, 'src/assets/'), 
      'stores': path.resolve(__dirname, 'src/stores/'),
      'utils': path.resolve(__dirname, 'src/utils/'),
      'router': path.resolve(__dirname, 'src/router.js'),
    },
    extensions: ['.js', '.vue']
  },
  module: {
    rules: [
      {
        test: /\.vue$/,
        loader: 'vue-loader'
      },
      {
        test: /\.styl(us)?$/i,
        use: [
          extractCSS.loader,
          {
            loader: 'css-loader',
            options: {
              sourceMap: DEV_MODE
            }
          },
          {
            loader: 'stylus-loader',
            options: {
              sourceMap: DEV_MODE
            }
          }
        ],
      },
      {
        test: /\.css$/i,
        use: [
          extractCSS.loader, 
          {
            loader: 'css-loader',
            options: {
              sourceMap: DEV_MODE
            }
          }
        ],
      },
      {   
        test: /\.(gif|png|jpe?g|svg)$/i,
        use: [
          {
            loader: 'file-loader',
            options: {
              name: 'assets/[name].[ext]'
            }
          }
        ]
      }
    ]
  },
  plugins: [
    new ESLint({
      extensions: ['js', 'vue']
    }),
    new SLint({
      files: ['**/*.{vue,css,stylus}'],
    }),
    new VueLoaderPlugin(),
    new extractCSS(),
    new html({
      templateContent: ({htmlWebpackPlugin}) => `
                <!DOCTYPE html>
                <html>
                <head>
                <meta charset="utf-8"/>
                <title>${htmlWebpackPlugin.options.title}</title>
                </head>
                <body>
                </body>
                </html>
            `,
      title: 'BEAM NFT Gallery',
      favicon: './src/appicon.svg',
      scriptLoading: 'blocking',
      meta: {
        viewport: 'width=device-width,initial-scale=1.0',
      },
    }),
    new copy({
      patterns: [
        './src/galleryManager.wasm',
        './src/appicon.svg',
        {   
          from: 'assets/**/*', 
          context: './src'
        },
        {
          from: path.join(__dirname, './node_modules/beam-wasm-client-masternet/'),
          globOptions: {
            ignore: ['**/package.json', '**/README.md'],
          },
        }
      ]
    })
  ],
  devtool: DEV_MODE ? 'eval-source-map' : undefined
}}

module.exports = (env, argv) => {
  const mode = argv.mode
  console.log('Mode:', mode)
  const DEV_MODE = mode == 'development'
  return config(DEV_MODE)
}
