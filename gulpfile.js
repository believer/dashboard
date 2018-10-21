const gulp = require('gulp')
const uglify = require('gulp-uglify')
const rename = require('gulp-rename')
const pump = require('pump')
const clean = require('gulp-clean')
const CacheBuster = require('gulp-cachebust')

const cachebust = new CacheBuster()

const buildJs = cb => {
  return pump(
    [
      gulp.src('dist/index.js'),
      uglify(),
      cachebust.resources(),
      gulp.dest('dist'),
    ],
    cb
  )
}

const buildHtml = cb => {
  return pump(
    [
      gulp.src('public/index.prod.html'),
      cachebust.references(),
      rename('index.html'),
      gulp.dest('dist'),
    ],
    cb
  )
}

const cleanup = () => {
  return pump([gulp.src('dist/index.js'), clean()])
}

gulp.task('build', gulp.series(buildJs, buildHtml, cleanup))
