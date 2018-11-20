let component = ReasonReact.statelessComponent("ExternalLink");

let make = (~href, ~target="_blank", children) => {
  ...component,
  render: _self =>
    <a className="link dark-blue hover-hot-pink" href target> ...children </a>,
};