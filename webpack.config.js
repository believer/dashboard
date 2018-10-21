const path = require('path')
const outputDir = path.join(__dirname, 'dist/')
const HtmlWebpackPlugin = require('html-webpack-plugin')

module.exports = {
  entry: './lib/es6/src/Index.bs.js',
  mode: 'production',
  output: {
    path: outputDir,
    publicPath: '/',
    filename: 'index.[hash:8].js',
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'public/index.html',
    }),
  ],
}
