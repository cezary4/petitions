<?php

/**
 * @file
 * Defines Petition class.
 */

/**
 * Class Petition.
 */
class Petition {

  protected $id;
  protected $uid;
  protected $title;
  protected $body;
  protected $issues;
  protected $signaturePublicThreshold;
  protected $signatureThreshold;
  protected $signatureCount;
  protected $signaturesNeeded;
  protected $reviewTimeframe;
  protected $niceUrl;
  protected $shortUrl;
  protected $deadline;
  protected $status;
  protected $reachedPublic;
  protected $created;
  protected $response;
  protected $responseStatus;
  protected $userTags;

  /**
   * Sets $this->body.
   *
   * @param string $body
   *   The petition body.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setBody($body) {
    $this->body = $body;

    return $this;
  }

  /**
   * Get $this->body.
   *
   * @return string
   *   The petition body.
   */
  public function getBody() {
    return petitions_data_sanitize_output($this->body);
  }

  /**
   * Sets $this->created.
   *
   * @param int $created
   *   The UNIX timestamp when this petition was created.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setCreated($created) {
    $this->created = $created;

    return $this;
  }

  /**
   * Gets $this->created.
   *
   * @return int
   *   The UNIX timestamp when this petition was created.
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   * Sets $this->deadline.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  protected function setDeadline() {
    $this->deadline = $this->calcDeadline($this->getCreated(), $this->getReviewTimeframe());

    return $this;
  }

  /**
   * Gets $this->deadline.
   *
   * @return int
   *   The UNIX timestamp when this petition no longer accepts signatures.
   */
  public function getDeadline() {
    if (!$this->deadline) {
      $this->setDeadline();
    }

    return $this->deadline;
  }

  /**
   * Sets $this->id.
   *
   * @param string $id
   *   The petition id.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setId($id) {
    $this->id = $id;

    return $this;
  }

  /**
   * Gets $this->id.
   *
   * @return string
   *   The petition id.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Sets $this->issues.
   *
   * @param array $issues
   *   An array of issues, containing the following keys:
   *   - id: The issue id.
   *   - name: The issue name.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setIssues(array $issues) {
    $this->issues = $issues;

    return $this;
  }

  /**
   * Gets $this->issues.
   *
   * @return array
   *   An associative array of issues.
   */
  public function getIssues() {
    return $this->issues;
  }

  /**
   * Sets $this->reviewTimeframe.
   *
   * @param int $review_timeframe
   *   The length of time in seconds between when a petition reaches threshold
   *   and when a response must be posted (business rule).
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setReviewTimeframe($review_timeframe) {
    $this->reviewTimeframe = $review_timeframe;

    return $this;
  }

  /**
   * Gets $this->reviewTimeframe.
   *
   * @return int
   *   The petition review timeframe in seconds.
   */
  public function getReviewTimeframe() {
    return $this->reviewTimeframe;
  }

  /**
   * Sets $this->signaturePublicThreshold.
   *
   * @param int $signature_public_threshold
   *   The number of signatures required for this petition to become public.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setSignaturePublicThreshold($signature_public_threshold) {
    $this->signaturePublicThreshold = $signature_public_threshold;

    return $this;
  }

  /**
   * Gets $this->signaturePublicThreshold.
   *
   * @return int
   *   The number of signatures required for this petition to become public.
   */
  public function getSignaturePublicThreshold() {
    return $this->signaturePublicThreshold;
  }

  /**
   * Sets $this->signatureThreshold.
   *
   * @param int $signature_threshold
   *   The number of signatures required for this petition to get a response.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setSignatureThreshold($signature_threshold) {
    $this->signatureThreshold = $signature_threshold;

    return $this;
  }

  /**
   * Gets $this->signatureThreshold.
   *
   * @return int
   *   The number of signatures required for this petition to get a response.
   */
  public function getSignatureThreshold() {
    return $this->signatureThreshold;
  }

  /**
   * Sets $this->signatureCount.
   *
   * @param int $signature_count
   *   The nubmer of signatures submitted to this petition.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setSignatureCount($signature_count) {
    $this->signatureCount = $signature_count;

    return $this;
  }

  /**
   * Gets $this->signatureCount.
   *
   * @return int
   *   The nubmer of signatures submitted to this petition.
   */
  public function getSignatureCount() {
    return $this->signatureCount;
  }

  /**
   * Sets $this->signaturesNeeded.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  protected function setSignaturesNeeded() {
    $this->signaturesNeeded = $this->calcSignaturesNeeded($this->getSignatureThreshold(), $this->getSignatureCount());

    return $this;
  }

  /**
   * Gets $this->signaturesNeeded.
   *
   * @return int
   *   The number of signatures needed for this petition to get a response.
   */
  public function getSignaturesNeeded() {
    if (!isset($this->signaturesNeeded)) {
      $this->setSignaturesNeeded();
    }
    return $this->signaturesNeeded;
  }

  /**
   * Sets $this->status.
   *
   * @param int $status
   *   The integer representing petition status. Should use constants defined
   *   in wh_petition module.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setStatus($status) {
    $this->status = $status;

    return $this;
  }

  /**
   * Gets $this->status.
   *
   * @return int
   *   The petition status.
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Helper function to translate status codes into readable string formats.
   *
   * @param int $status_code
   *   Status code as stored.
   *
   * @return string
   *   String representing status code.
   */
  static public function formatStatus($status_code) {
    switch ($status_code) {
      // Published, but does not have the required number of signatures to
      // become public, only visible by direct URL.
      case WH_PETITION_STATUS_PRIVATE:
        // Has the required number of signatures to become public,
        // visible everywhere on site.
      case WH_PETITION_STATUS_PUBLIC:
        return "open";

      // Has received the required number of signatures, visible on site.
      case WH_PETITION_STATUS_UNDER_REVIEW:
        return "pending response";

      // Has received a response.
      case WH_PETITION_STATUS_REVIEWED:
        return "responded";

      default:
        return "closed";
    }
  }

  /**
   * Sets $this->title.
   *
   * @param string $title
   *   The petition title.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setTitle($title) {
    $this->title = $title;

    return $this;
  }

  /**
   * Gets $this->title.
   *
   * @return string
   *   The petition title.
   */
  public function getTitle() {
    return petitions_data_sanitize_output($this->title);
  }

  /**
   * Sets $this->niceUrl.
   *
   * @param string $url
   *   The petition URL.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setNiceUrl($url) {
    $this->niceUrl = $url;

    return $this;
  }

  /**
   * Gets $this->niceUrl.
   *
   * @return string
   *   The petition URL.
   */
  public function getNiceUrl() {
    global $base_url;
    $website_url = rtrim(variable_get('petitions_data_petitions_website_url', $base_url), '/');

    return url(ltrim($this->niceUrl, '/'), array('absolute' => TRUE, 'base_url' => $website_url));
  }

  /**
   * Sets $this->shortUrl.
   *
   * @param string $url
   *   The petition URL.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setShortUrl($url) {
    $this->shortUrl = $url;

    return $this;
  }

  /**
   * Gets $this->shortUrl.
   *
   * @return string
   *   The petition URL.
   */
  public function shortUrl() {
    return $this->shortUrl;
  }

  /**
   * Sets $this->reachedPublic.
   *
   * @param int $reached_public
   *   A UNIX timestamp representing when this petition gained public status.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setReachedPublic($reached_public) {
    $this->reachedPublic = $reached_public;

    return $this;
  }

  /**
   * Gets $this->reachedPublic.
   *
   * @return int
   *   A UNIX timestamp representing when this petition gained public status.
   */
  public function getReachedPublic() {
    return $this->reachedPublic;
  }

  /**
   * Sets $this->response.
   *
   * @param object $response
   *   The response to the petition.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setResponse($response) {
    $this->response = $response;

    return $this;
  }

  /**
   * Gets $this->response.
   *
   * @return object
   *   The response to the petition.
   */
  public function getResponse() {
    return $this->response;
  }

  /**
   * Sets $this->responseStatus.
   *
   * @param object $response_status
   *   Indicates whether this petition has a response.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setResponseStatus($response_status) {
    $this->responseStatus = $response_status;

    return $this;
  }

  /**
   * Gets $this->responseStatus.
   *
   * @return bool
   *   The whether the petition has a response.
   */
  public function getResponseStatus() {
    return $this->responseStatus;
  }

  /**
   * Sets $this->uid.
   *
   * @param int $uid
   *   The user id of the signatory.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setUid($uid) {
    $this->uid = $uid;

    return $this;
  }

  /**
   * Gets $this->uid.
   *
   * @return int
   *   The user id of the signatory.
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Sets $this->userTags.
   *
   * @param array $user_tags
   *   An array of tags for this petition, created by petition creator.
   *
   * @return Petition
   *   Returns current instance of object.
   */
  public function setUserTags(array $user_tags) {
    $this->userTags = $user_tags;

    return $this;
  }

  /**
   * Gets $this->userTags.
   *
   * @return array
   *   An array of tags for this petition.
   */
  public function getUserTags() {
    return $this->userTags;
  }


  /**
   * Helper function to calculate needed signatures.
   *
   * @param int $threshold
   *   Signatures required to solicit a response.
   *
   * @param int $current
   *   Current number of signatures.
   *
   * @return int
   *   Signatures remaining to required to reach response threshold.
   */
  public static function calcSignaturesNeeded($threshold, $current) {
    return ($current >= $threshold) ? 0 : $threshold - $current;
  }

  /**
   * Helper function to calculate deadline.
   *
   * @param int $created
   *   Epoch (UNIX style) time stamp.
   *
   * @param int $days
   *   Number of days from creation that the Petition will remain open.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  public static function calcDeadline($created, $days) {
    return strtotime("+{$days} days", $created);
  }

  /**
   * Converts into a publicly consumable array.
   *
   * @return array
   *   An array to be used for public display.
   */
  public function toArray() {
    $output = array(
      'id' => $this->getId(),
      'type' => 'petition',
      'title' => $this->getTitle(),
      'body' => $this->getBody(),
      'issues' => $this->getIssues(),
      'signatureThreshold' => $this->getSignatureThreshold(),
      'signatureCount' => $this->getSignatureCount(),
      'signaturesNeeded' => $this->getSignaturesNeeded(),
      'url' => $this->getNiceUrl(),
      'deadline' => $this->getDeadline(),
      'status' => $this->formatStatus($this->getStatus()),
      'response' => $this->getResponse(),
      'created' => $this->getCreated(),
    );

    return $output;
  }
}
