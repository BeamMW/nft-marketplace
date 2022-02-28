const path = require('path')
const html = require('html-webpack-plugin')
const copy = require("copy-webpack-plugin")
const extractCSS = require("mini-css-extract-plugin")
const {VueLoaderPlugin} = require("vue-loader")

const config = (DEV_MODE) => {return {
    entry: {
        main: "./src/main.js",
    },
    devServer: {
        hot: true,
        headers: {
            "Cross-Origin-Opener-Policy": "same-origin",
            "Cross-Origin-Embedder-Policy": "require-corp"
        }
    },
    output: {
        filename: "[name].bundle.js",
        path: path.resolve(__dirname, "html"),
        clean: true
    },
    module: {
        rules: [
            {
                test: /\.vue$/,
                loader: 'vue-loader'
            },
            {
                test: /\.(jpe?g|png|svg)(\?[a-z0-9=.]+)?$/,
                use: [
                    {
                        loader: 'url-loader',
                    }
                ]
            },
            {
                test: /\.styl(us)?$/i,
                use: [
                    extractCSS.loader,
                    {
                        loader: "css-loader",
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
                        loader: "css-loader",
                        options: {
                            sourceMap: DEV_MODE
                        }
                    }
                ],
            }
        ]
    },
    plugins: [
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
            title: "BEAM NFT Gallery",
            favicon: "./src/appicon.svg",
            scriptLoading: "blocking",
            meta: {
                viewport: "width=device-width,initial-scale=1.0",
            },
        }),
        new copy({
            patterns: [
                "./src/galleryManager.wasm",
                "./src/appicon.svg",
                {   
                    from: "assets/**/*", 
                    context: "./src"
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
    devtool: DEV_MODE ? "eval-source-map" : undefined
}}

module.exports = (env, argv) => {
    const DEV_MODE = argv.mode == 'development'
    console.log(argv.mode)
    return config(DEV_MODE)
}
