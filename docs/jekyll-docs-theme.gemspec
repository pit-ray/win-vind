# frozen_string_literal: true
# coding: utf-8

Gem::Specification.new do |spec|
  spec.name          = "jekyll-docs-theme"
  spec.version       = "1.0.0"
  spec.authors       = ["Vladimir 'allejo' Jimenez"]
  spec.email         = ["me@allejo.io"]

  spec.summary       = "A Jekyll Gem-based Theme for hosting documentation style websites"
  spec.homepage      = "https://github.com/allejo/jekyll-docs-theme"
  spec.license       = "MIT"

  spec.metadata["plugin_type"] = "theme"

  spec.files = `git ls-files -z`.split("\x0").select do |f|
    f.match(%r!^(assets|_(includes|layouts|sass)/|(LICENSE|README)((\.(txt|md|markdown)|$)))!i)
  end

  spec.add_runtime_dependency "jekyll", ">= 3.5", "< 5.0"

  spec.add_development_dependency "bundler", "~> 2.1"
end
