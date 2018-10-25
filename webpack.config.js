const path = require('path')
const outputDir = path.join(__dirname, 'dist/')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const WebpackPwaManifest = require('webpack-pwa-manifest')
const CopyWebpackPlugin = require('copy-webpack-plugin')

module.exports = {
  entry: './lib/es6/src/Index.bs.js',
  mode: 'production',
  output: {
    path: outputDir,
    publicPath: '/',
    filename: 'index.[hash:8].js',
  },
  plugins: [
    new CopyWebpackPlugin([
      {
        from: path.join(__dirname, './public/images/*'),
        to: '',
        flatten: true,
      },
    ]),
    new HtmlWebpackPlugin({
      template: 'public/index.html',
    }),
    new WebpackPwaManifest({
      inject: true,
      name: 'Dashboard',
      short_name: 'Dash',
      description: 'Tiny dashboard for Trello and GitHub notifications',
      background_color: '#00449E',
      theme_color: '#00449E',
      icons: [
        {
          src: path.resolve('public/images/logo@3x.png'),
          sizes: [96, 128, 192, 256, 384, 512, 1024],
        },
      ],
    }),
  ],
}
